#include "text.hpp"
#include "asset_manager.hpp"
#include "game.hpp"

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

void DynamicText::Init()
{
    char chars[128] = {};
    for (int i = 0; i < 127; i++)
    {
        chars[i] = 32 + i;
    }

    SDL_Surface* surf = TTF_RenderUTF8_Blended(Asset::GetFont(), chars, {255, 255, 255, 255});

    char offset[128] = {};
    for (int i = 0; i < 127; i++)
    {
        char single[2] = {};
        single[0] = chars[i];
        int w, h, ow, oh;
        TTF_SizeUTF8(Asset::GetFont(), offset, &ow, &oh);
        TTF_SizeUTF8(Asset::GetFont(), single, &w, &h);
        atlas[i] = {ow, 0, w, h};
        offset[i] = chars[i];
    }

    mTexture = SDL_CreateTextureFromSurface(Game::GetRenderer(), surf);

    SDL_FreeSurface(surf);

    lineSkip = TTF_FontLineSkip(Asset::GetFont());
}

void DynamicText::Draw(int x, int y, const std::string &text)
{
    auto ctext = text.c_str();

    int hpos = 0;
    int vpos = y;
    for (size_t i = 0; i < text.length(); i++)
    {
        char c = ctext[i];
        if (c == '\n')
        {
            vpos += lineSkip;
            hpos = 0;
        }
        else if (c == '\t')
        {
            hpos += tabWidth * atlas[' ' - 32].w;
        }
        
        if (c < 32) {
            continue;
        }

        SDL_Rect src = atlas[c - 32];
        SDL_Rect dest = {
            .x = x + hpos,
            .y = vpos,
            .w = src.w,
            .h = src.h
        };

        SDL_RenderCopy(Game::GetRenderer(), mTexture, &src, &dest);

        hpos += src.w;
    }
}

void DynamicText::Destroy()
{
    SDL_DestroyTexture(mTexture);
}
