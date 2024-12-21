#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.hpp"

class Game;

class StaticText{
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

class DynamicText{
public:
    void Init();
    void Draw(int x, int y, const std::string& text);
    void Destroy();
    SDL_Texture* mTexture = nullptr;
    SDL_Rect atlas[127] = {};
    int lineSkip = 0;
    int tabWidth = 4;
};

#endif