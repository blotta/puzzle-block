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

enum class VerticalAlign
{
    TOP,
    MIDDLE,
    BOTTOM
};

struct FontDrawOptions
{
    SDL_Color color = {255, 255, 255, 255};
    TextAlign align = TextAlign::LEFT;
    VerticalAlign valign = VerticalAlign::TOP;
    int lineHeight = 0;
};

struct TextSize
{
    int width;
    int height;
};

class Font
{
  public:
    Font(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    Font(const Font& other) = delete; // disallows copying
    Font(Font&& other) noexcept;      // move constructor
    ~Font();
    void drawText(int x, int y, const std::string& text, const FontDrawOptions& options = {}) const;
    void drawTextLine(int x, int y, const std::string& text, const FontDrawOptions& options = {}) const;

    TextSize measureText(const std::string& text, const FontDrawOptions& options = {}) const;

    std::string fontPath;
    int fontSize;

  private:
    struct Glyph
    {
        SDL_Rect src;
        int advance;
        int offsetX, offsetY;
    };

    SDL_Renderer* pRenderer = nullptr;
    TTF_Font* mFont = nullptr;
    SDL_Texture* mAtlas = nullptr;
    std::unordered_map<char, Glyph> mCharData;

    void generateAtlas();
};

#endif