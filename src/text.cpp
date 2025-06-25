#include "text.hpp"
#include "asset_manager.hpp"
#include "game.hpp"

FontAtlas::FontAtlas(SDL_Renderer* renderer, const std::string& fontPath, int fontSize)
    : fontPath(fontPath), fontSize(fontSize), renderer(renderer), font(nullptr), atlas(nullptr)
{
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font)
        return;

    generateAtlas();
}

FontAtlas::~FontAtlas()
{
    if (atlas)
        SDL_DestroyTexture(atlas);
    if (font)
        TTF_CloseFont(font);
}

void FontAtlas::drawText(int x, int y, const std::string& text, SDL_Color color, TextAlign align)
{
    if (!atlas)
        return;

    int totalWidth = 0;
    for (char c : text)
    {
        totalWidth += charData[c].advance;
    }

    int drawX = x;
    if (align == TextAlign::CENTER)
        drawX -= totalWidth / 2;
    else if (align == TextAlign::RIGHT)
        drawX -= totalWidth;

    for (char c : text)
    {
        const Glyph& g = charData[c];
        SDL_Rect src = g.src;
        SDL_Rect dst = {drawX + g.offsetX, y + g.offsetY, src.w, src.h};
        SDL_SetTextureColorMod(atlas, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(atlas, color.a);
        SDL_RenderCopy(renderer, atlas, &src, &dst);
        drawX += g.advance;
    }
}

void FontAtlas::generateAtlas()
{
    const std::string chars =
        " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    SDL_Color white = {255, 255, 255, 255};
    int spacing = 2;
    int atlasWidth = 0, atlasHeight = 0;

    std::vector<SDL_Surface*> glyphSurfaces;
    for (char c : chars)
    {
        SDL_Surface* surf = TTF_RenderGlyph_Blended(font, c, white);
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
        TTF_GlyphMetrics32(font, c, &minx, &maxx, &miny, &maxy, &advance);

        this->charData[c] = {.src = dst, .advance = advance, .offsetX = minx, .offsetY = 0};

        xOffset += surf->w + spacing;
        SDL_FreeSurface(surf);
    }

    atlas = SDL_CreateTextureFromSurface(renderer, atlasSurface);
    SDL_FreeSurface(atlasSurface);

    SDL_Log("FontAtlas texture generated for %s:%d\n", fontPath.c_str(), fontSize);
}
