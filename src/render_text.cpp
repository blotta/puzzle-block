#include "render_text.hpp"

RenderText::RenderText(TTF_Font* font)
{
    pFont = font;
    setText(mText);
    setColor(mColor);
}

RenderText::~RenderText()
{
    SDL_DestroyTexture(mTextTexture);
}

void RenderText::setText(std::string text)
{
    if (text.compare(mText) == 0)
        return;

    mText = text;
    mPropsChanged = true;
}

void RenderText::setColor(SDL_Color color)
{
    if (color.r == mColor.r && color.g == mColor.g && color.b == mColor.b && color.a == mColor.a)
        return;
    
    mColor = color;
    mPropsChanged = true;
}

void RenderText::draw(SDL_Renderer* rend, int x, int y)
{
    if (!sync(rend))
        return;

    SDL_Rect r = {
        x, y,
        mWidth, mHeight
    };
    SDL_RenderCopy(rend, mTextTexture, NULL, &r);
}

bool RenderText::sync(SDL_Renderer* rend)
{
    if (!mPropsChanged)
        return true;
    
    if (mText.empty())
        return false;
    
    SDL_Surface* surf = TTF_RenderUTF8_Blended(pFont, mText.c_str(), mColor);
    if (!surf)
    {
        SDL_Log("Error generating text surface: %s\n", TTF_GetError());
        return false;
    }
    mTextTexture = SDL_CreateTextureFromSurface(rend, surf);
    if (!mTextTexture)
    {
        SDL_Log("Error creating text texture: %s\n", SDL_GetError());
        return false;
    }
    mWidth = surf->w;
    mHeight = surf->h;
    SDL_FreeSurface(surf);

    mPropsChanged = false;
    return true;
}
