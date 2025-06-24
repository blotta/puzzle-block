#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class StaticText
{
  public:
    StaticText();
    ~StaticText();
    StaticText(const StaticText&) = delete;
    StaticText(StaticText&& other) noexcept; // move constructor
    void clearText();
    const std::string& getText();
    void setText(std::string text);
    void setColor(SDL_Color color);
    void setPointSize(float ptSize);
    void draw(int x, int y);
    int getWidth();
    int getHeight();
    float scale = 1.0f;
    int hAlign = 0;

  private:
    std::string mText;
    SDL_Color mColor = {255, 255, 255, 255};
    SDL_Texture* mTextTexture = NULL;
    int width, height = 0;
    bool mPropsChanged = true;
    bool sync();
};

class DynamicText
{
  public:
    void Init();
    void Draw(int x, int y, const std::string& text);
    void Destroy();
    SDL_Texture* mTexture = nullptr;
    SDL_Rect atlas[127] = {};
    int lineSkip = 0;
    int tabWidth = 4;
};

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