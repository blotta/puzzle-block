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
    void setText(std::string text);
    void setColor(SDL_Color color);
    void draw(SDL_Renderer* rend, int x, int y);
    int getWidth();
    int getHeight();
    int width, height = 0;
    float scale = 1.0f;
private:
    std::string mText;
    SDL_Color mColor = {255, 255, 255, 255};
    SDL_Texture* mTextTexture = NULL;
    bool mPropsChanged = true;
    bool sync(SDL_Renderer* rend);
};

#endif