#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum class TextAlign
{
    LEFT,
    CENTER,
    RIGHT
};

class FontAtlas
{
  public:
    FontAtlas(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    ~FontAtlas();
    void drawText(int x, int y, const std::string& text, SDL_Color color = {255, 255, 255, 255},
                  TextAlign align = TextAlign::LEFT);

    std::string fontPath;
    int fontSize;

  private:
    struct Glyph
    {
        SDL_Rect src;
        int advance;
        int offsetX, offsetY;
    };

    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* atlas;
    std::unordered_map<char, Glyph> charData;

    void generateAtlas();
};

#endif