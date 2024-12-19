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
    this->width = other.width;
    this->height = other.height;
    this->mTextTexture = other.mTextTexture;

    other.mTextTexture = nullptr;
}

void StaticText::clearText()
{
    mText.clear();
    mPropsChanged = true;
}

const std::string &StaticText::getText()
{
    return mText;
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

void StaticText::setPointSize(float ptSize)
{
    int currPtSize = Asset::GetFontPointSize();
    scale = (float)ptSize / (float)currPtSize;
}

void StaticText::draw(int x, int y)
{
    if (!sync())
        return;

    SDL_Rect r = {
        x, y,
        (int)SDL_floor(width * scale),
        (int)SDL_floor(height * scale)
    };

    if (hAlign == 1)
        r.x = r.x - (int)SDL_floor(width/2 * scale);
    if (hAlign == 2)
        r.x = r.x - (int)SDL_floor(width * scale);
    SDL_RenderCopy(Game::GetRenderer(), mTextTexture, NULL, &r);
}

int StaticText::getWidth() { return width; }

int StaticText::getHeight() { return height; }

bool StaticText::sync()
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
    mTextTexture = SDL_CreateTextureFromSurface(Game::GetRenderer(), surf);
    if (!mTextTexture)
    {
        SDL_Log("Error creating text texture: %s\n", SDL_GetError());
        return false;
    }
    width = surf->w;
    height = surf->h;
    SDL_FreeSurface(surf);

    mPropsChanged = false;
    return true;
}
