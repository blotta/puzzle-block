#include "font.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "log.hpp"

Font::Font(SDL_Renderer* renderer, const std::string& fontPath, int fontSize)
    : fontPath(fontPath), fontSize(fontSize), pRenderer(renderer), mFont(nullptr), mAtlas(nullptr)
{
    mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!mFont)
        return;

    generateAtlas();
}

Font::Font(Font&& other) noexcept
    : fontPath(std::move(other.fontPath)), fontSize(other.fontSize), pRenderer(other.pRenderer), mFont(other.mFont),
      mAtlas(other.mAtlas), mCharData(std::move(other.mCharData))
{

    other.pRenderer = nullptr;
    other.mFont = nullptr;
    other.mAtlas = nullptr;
    Log::debug("Moving font\n");
}

Font::~Font()
{
    if (mAtlas)
        SDL_DestroyTexture(mAtlas);
    if (mFont)
        TTF_CloseFont(mFont);
}

void Font::drawText(int x, int y, const std::string& text, SDL_Color color, TextAlign align) const
{
    if (!mAtlas)
        return;

    int totalWidth = 0;
    for (char c : text)
    {
        totalWidth += mCharData.at(c).advance;
    }

    int drawX = x;
    if (align == TextAlign::CENTER)
        drawX -= totalWidth / 2;
    else if (align == TextAlign::RIGHT)
        drawX -= totalWidth;

    for (char c : text)
    {
        const Glyph& g = mCharData.at(c);
        SDL_Rect src = g.src;
        SDL_Rect dst = {drawX + g.offsetX, y + g.offsetY, src.w, src.h};
        SDL_SetTextureColorMod(mAtlas, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(mAtlas, color.a);
        SDL_RenderCopy(pRenderer, mAtlas, &src, &dst);
        drawX += g.advance;
    }
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

        this->mCharData[c] = {.src = dst, .advance = advance, .offsetX = minx, .offsetY = 0};

        xOffset += surf->w + spacing;
        SDL_FreeSurface(surf);
    }

    mAtlas = SDL_CreateTextureFromSurface(pRenderer, atlasSurface);
    SDL_FreeSurface(atlasSurface);

    Log::info("Font texture generated for %s:%d\n", fontPath.c_str(), fontSize);
}
