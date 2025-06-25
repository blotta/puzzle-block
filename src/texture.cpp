
#include "texture.hpp"
#include "log.hpp"
#include <SDL2/SDL_image.h>

Texture::Texture(SDL_Renderer* rend, const std::string& path)
{
    Log::info("Creating texture %s\n", path.c_str());
    mTexture = IMG_LoadTexture(rend, path.c_str());
    if (!mTexture)
    {
        Log::error("Couldn't load texture from file %s: %s\n", path.c_str(), SDL_GetError());
    }
    int w, h;
    int status = SDL_QueryTexture(mTexture, NULL, NULL, &w, &h);
    Log::debug("status %d\n", status);
}

Texture::Texture(Texture&& other) noexcept
{
    Log::debug("Moving texture\n");
    mTexture = other.mTexture;

    other.mTexture = nullptr;
}

Texture::~Texture()
{
    Log::debug("Deleting texture\n");
    if (mTexture != nullptr)
    {
        Log::debug("Destroying SDL texture\n");
        int status = SDL_QueryTexture(mTexture, NULL, NULL, NULL, NULL);
        if (status < 0)
        {
            Log::error("SDL_QueryTexture failed with status %d: %s\n", status, SDL_GetError());
        }

        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

SDL_Texture* Texture::get() const
{
    return mTexture;
}