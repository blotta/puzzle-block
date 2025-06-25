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

class Font
{
  public:
    Font(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    Font(const Font& other) = delete; // disallows copying
    Font(Font&& other) noexcept; // move constructor
    ~Font();
    void drawText(int x, int y, const std::string& text, SDL_Color color = {255, 255, 255, 255},
                  TextAlign align = TextAlign::LEFT) const;

    std::string fontPath;
    int fontSize;

  private:
    struct Glyph
    {
        SDL_Rect src;
        int advance;
        int offsetX, offsetY;
    };

    SDL_Renderer* pRenderer;
    TTF_Font* mFont;
    SDL_Texture* mAtlas;
    std::unordered_map<char, Glyph> mCharData;

    void generateAtlas();
};

#endif