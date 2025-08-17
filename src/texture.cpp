
#include "texture.hpp"
#include "log.hpp"
#include <SDL2/SDL_image.h>

Texture::Texture(SDL_Renderer* rend, const std::string& path) : path(std::move(path)), pRenderer(rend)
{
    Log::info("Creating texture %s\n", path.c_str());
    mTexture = IMG_LoadTexture(pRenderer, path.c_str());
    if (!mTexture)
    {
        Log::error("Couldn't load texture from file %s: %s\n", path.c_str(), SDL_GetError());
    }
    int w, h;
    int status = SDL_QueryTexture(mTexture, NULL, NULL, &w, &h);
}

Texture::Texture(Texture&& other) noexcept
    : path(std::move(other.path)), pRenderer(other.pRenderer), mTexture(other.mTexture)
{
    other.pRenderer = nullptr;
    other.mTexture = nullptr;

    Log::debug("Moving texture %s\n", path.c_str());
}

Texture::~Texture()
{
    if (mTexture != nullptr)
    {
        Log::debug("Destroying SDL texture\n");
        int status = SDL_QueryTexture(mTexture, NULL, NULL, NULL, NULL);
        if (status < 0)
        {
            Log::error("SDL_QueryTexture failed with status %d: %s\n", status, SDL_GetError());
        }

        Log::debug("Deleting texture %s\n", path.c_str());
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

SDL_Texture* Texture::get() const
{
    return mTexture;
}

void Texture::drawTile(const SDL_Rect* src, const SDL_Rect* dest) const
{
    SDL_RenderCopy(pRenderer, mTexture, src, dest);
}
