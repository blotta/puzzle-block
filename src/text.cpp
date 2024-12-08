#include "text.hpp"

StaticText::StaticText()
{
    setText(mText);
    setColor(mColor);
    SDL_Log("Create Static Text\n");
}

StaticText::~StaticText()
{
    SDL_DestroyTexture(mTextTexture);
    SDL_Log("Destroy Static Text\n");
}

StaticText::StaticText(StaticText&& other) noexcept
{
    this->mText = other.mText;
    this->mColor = other.mColor;
    this->mPropsChanged = other.mPropsChanged;
    this->mWidth = other.mWidth;
    this->mHeight = other.mHeight;
    this->mTextTexture = other.mTextTexture;

    other.mTextTexture = nullptr;
}

void StaticText::clearText()
{
    mText.clear();
    mPropsChanged = true;
}

void StaticText::setText(std::string text)
{
    if (text.compare(mText) == 0)
        return;

    mText = text;
    mPropsChanged = true;
}

void StaticText::setColor(SDL_Color color)
{
    if (color.r == mColor.r && color.g == mColor.g && color.b == mColor.b && color.a == mColor.a)
        return;
    
    mColor = color;
    mPropsChanged = true;
}

void StaticText::draw(SDL_Renderer* rend, int x, int y)
{
    if (!sync(rend))
        return;

    SDL_Rect r = {
        x, y,
        mWidth, mHeight
    };
    SDL_RenderCopy(rend, mTextTexture, NULL, &r);
}

int StaticText::getWidth() { return mWidth; }

int StaticText::getHeight() { return mHeight; }

bool StaticText::sync(SDL_Renderer* rend)
{
    if (!mPropsChanged && mTextTexture != NULL)
        return true;
    
    if (mText.empty())
    {
        if (mTextTexture != NULL)
        {
            SDL_DestroyTexture(mTextTexture);
            mTextTexture = NULL;
        }
        return false;
    }
    
    SDL_Surface* surf = TTF_RenderUTF8_Blended(Asset::GetFont(), mText.c_str(), mColor);
    if (!surf)
    {
        SDL_Log("Error generating text surface: %s\n", TTF_GetError());
        return false;
    }
    if (mTextTexture != NULL)
    {
        SDL_DestroyTexture(mTextTexture);
        mTextTexture = NULL;
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
