#include "font.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "log.hpp"
#include <sstream>

static inline int kerningPx(TTF_Font* f, Uint16 prevIndex, Uint16 index)
{
    if (!prevIndex || !index)
        return 0;

    return TTF_GetFontKerningSizeGlyphs(f, prevIndex, index);
}

Font::Font(SDL_Renderer* renderer, const std::string& fontPath, int fontSize)
    : fontPath(fontPath), fontSize(fontSize), pRenderer(renderer), mFont(nullptr), mAtlas(nullptr)
{
    mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!mFont)
        return;

    TTF_SetFontKerning(mFont, 1);

    generateAtlas();
}

Font::Font(Font&& other) noexcept
    : fontPath(std::move(other.fontPath)), fontSize(other.fontSize), pRenderer(other.pRenderer), mFont(other.mFont),
      mAtlas(other.mAtlas), mCharData(std::move(other.mCharData))
{

    other.pRenderer = nullptr;
    other.mFont = nullptr;
    other.mAtlas = nullptr;
}

Font::~Font()
{
    if (mAtlas != nullptr)
    {
        Log::debug("Destroying font atlas %s:%d\n", fontPath.c_str(), fontSize);
        SDL_DestroyTexture(mAtlas);
    }
    if (mFont != nullptr)
    {
        Log::debug("Destroying font %s:%d\n", fontPath.c_str(), fontSize);
        TTF_CloseFont(mFont);
    }
}

void Font::drawText(int x, int y, const std::string& text, const FontDrawOptions& options) const
{
    const int lineHeight = (options.lineHeight > 0) ? options.lineHeight : TTF_FontLineSkip(mFont);

    int cursorY = y;

    TextSize size = measureText(text, options);

    switch (options.valign)
    {
    case VerticalAlign::TOP:
        break;
    case VerticalAlign::MIDDLE:
        cursorY -= size.height / 2;
        break;
    case VerticalAlign::BOTTOM:
        cursorY -= size.height;
        break;
    }

    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line))
    {
        drawTextLine(x, cursorY, line, options);
        cursorY += lineHeight;
    }
}

void Font::drawTextLine(int x, int y, const std::string& text, const FontDrawOptions& options) const
{
    if (!mAtlas)
        return;

    int totalWidth = 0;
    Uint16 prevIndex = 0;
    for (char c : text)
    {
        if (c < 32 || c > 126)
        {
            prevIndex = 0;
            continue;
        }
        auto it = mCharData.find(c);
        if (it == mCharData.end())
        {
            prevIndex = 0;
            continue;
        }

        const Glyph& g = it->second;
        totalWidth += kerningPx(mFont, prevIndex, g.glyphIndex);
        totalWidth += g.advance;
        prevIndex = g.glyphIndex;
    }

    int drawX = x;
    if (options.align == TextAlign::CENTER)
        drawX -= totalWidth / 2;
    else if (options.align == TextAlign::RIGHT)
        drawX -= totalWidth;

    SDL_SetTextureColorMod(mAtlas, options.color.r, options.color.g, options.color.b);
    SDL_SetTextureAlphaMod(mAtlas, options.color.a);

    prevIndex = 0;
    for (char c : text)
    {
        if (c < 32 || c > 126)
        {
            prevIndex = 0;
            continue;
        }

        const Glyph& g = mCharData.at(c);

        drawX += kerningPx(mFont, prevIndex, g.glyphIndex);

        SDL_Rect src = g.src;
        SDL_Rect dst = {drawX + g.offsetX, y + g.offsetY, src.w, src.h};
        SDL_RenderCopy(pRenderer, mAtlas, &src, &dst);

        drawX += g.advance;
        prevIndex = g.glyphIndex;
    }
}

// Ignores kerning, as it was giving mixed results. Only sums "advance"
TextSize Font::measureText(const std::string& text, const FontDrawOptions& options) const
{
    if (!mAtlas)
        return {0, 0};

    const int lineHeight = (options.lineHeight > 0) ? options.lineHeight : TTF_FontLineSkip(mFont);

    int maxWidth = 0;
    int currentWidth = 0;
    int lines = 1;

    for (char c : text)
    {
        if (c == '\n')
        {
            maxWidth = std::max(maxWidth, currentWidth);
            currentWidth = 0;
            ++lines;
            continue;
        }

        if (c < 32 || c > 126)
        {
            continue;
        }

        auto it = mCharData.find(c);
        if (it == mCharData.end())
        {
            continue;
        }

        const Glyph& g = it->second;

        // currentWidth += kerningPx(mFont, prevIndex, g.glyphIndex);
        currentWidth += g.advance;
    }

    maxWidth = std::max(maxWidth, currentWidth);

    return {maxWidth, lines * lineHeight};
}

void Font::generateAtlas()
{
    const std::string chars =
        " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    SDL_Color white = {255, 255, 255, 255};
    int spacing = 2;
    int atlasWidth = 0, atlasHeight = 0;

    std::vector<SDL_Surface*> glyphSurfaces;
    for (char c : chars)
    {
        SDL_Surface* surf = TTF_RenderGlyph_Blended(mFont, c, white);
        if (!surf)
            continue;
        glyphSurfaces.push_back(surf);
        atlasWidth += surf->w + spacing;
        atlasHeight = std::max(atlasHeight, surf->h);
    }

    SDL_Surface* atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, atlasWidth, atlasHeight, 32, SDL_PIXELFORMAT_RGBA32);
    int xOffset = 0;
    size_t index = 0;
    for (char c : chars)
    {
        SDL_Surface* surf = glyphSurfaces[index++];
        if (!surf)
            continue;
        SDL_Rect dst = {xOffset, 0, surf->w, surf->h};
        SDL_BlitSurface(surf, nullptr, atlasSurface, &dst);

        int minx, maxx, miny, maxy, advance;
        TTF_GlyphMetrics32(mFont, c, &minx, &maxx, &miny, &maxy, &advance);
        Uint16 gindex = TTF_GlyphIsProvided32(mFont, c);

        this->mCharData[c] = {
            .src = dst,
            .advance = advance,
            .offsetX = minx,
            .offsetY = 0,
            .glyphIndex = gindex,
        };

        xOffset += surf->w + spacing;
        SDL_FreeSurface(surf);
    }

    mAtlas = SDL_CreateTextureFromSurface(pRenderer, atlasSurface);
    SDL_FreeSurface(atlasSurface);

    Log::debug("Font texture generated for %s:%d\n", fontPath.c_str(), fontSize);
}
