#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.hpp"

class Game;

class RenderText{
public:
    RenderText(TTF_Font* font);
    ~RenderText();
    void clearText();
    void setText(std::string text);
    void setColor(SDL_Color color);
    void draw(SDL_Renderer* rend, int x, int y);
    int getWidth();
    int getHeight();
private:
    TTF_Font* pFont;
    std::string mText;
    SDL_Color mColor = {255, 255, 255, 255};
    SDL_Texture* mTextTexture = NULL;
    bool mPropsChanged = true;
    int mWidth, mHeight = 0;
    bool sync(SDL_Renderer* rend);
};

#endif