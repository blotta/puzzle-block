#include "asset_manager.hpp"
#include "text.hpp"
#include <SDL2/SDL_image.h>
#include <format>

Asset& Asset::get()
{
    static Asset instance;
    return instance;
}

Asset::Asset()
{
    SDL_Log("Asset initialized\n");
}

void Asset::SetRenderer(SDL_Renderer* renderer)
{
    Asset::get().pRenderer = renderer;
}

void Asset::UnloadAssets()
{
    auto& mgr = Asset::get();
    mgr.mTextures.clear();

    if (mgr.mFont)
        TTF_CloseFont(mgr.mFont);

    for (const auto& [path, sfx] : mgr.mSounds)
    {
        Mix_FreeChunk(sfx);
    }

    for (const auto& [path, sfx] : mgr.mMusics)
    {
        Mix_FreeMusic(sfx);
    }

    for (const auto& [path, fa] : mgr.mFontAtlases)
    {
        delete fa;
    }
}

const Texture* Asset::GetTexture(const std::string& path)
{
    auto& mgr = Asset::get();
    auto it = mgr.mTextures.find(path);
    if (it == mgr.mTextures.end())
        LoadTexture(path);

    return &mgr.mTextures.at(path);
}

void Asset::LoadTexture(const std::string& path)
{
    auto& mgr = Asset::get();
    Texture t(mgr.pRenderer, path);
    mgr.mTextures.emplace(path, std::move(t));
}

Mix_Chunk* Asset::GetSound(const std::string& path)
{
    auto& mgr = Asset::get();
    auto it = mgr.mSounds.find(path);
    if (it == mgr.mSounds.end())
        LoadSound(path);

    return mgr.mSounds.at(path);
}

void Asset::LoadSound(const std::string& path)
{
    auto& mgr = Asset::get();
    auto sfx = Mix_LoadWAV(path.c_str());
    mgr.mSounds.emplace(path, sfx);
}

Mix_Music* Asset::GetMusic(const std::string& path)
{
    auto& mgr = Asset::get();
    auto it = mgr.mMusics.find(path);
    if (it == mgr.mMusics.end())
        LoadSound(path);

    return mgr.mMusics.at(path);
}

void Asset::LoadMusic(const std::string& path)
{
    auto& mgr = Asset::get();
    auto sfx = Mix_LoadMUS(path.c_str());
    mgr.mMusics.emplace(path, sfx);
}

void Asset::LoadFont(const std::string& path, int ptsize)
{
    auto& mgr = Asset::get();
    mgr.mFont = TTF_OpenFont(path.c_str(), ptsize);
    if (!mgr.mFont)
    {
        SDL_Log("Failed to load font: %s\n", TTF_GetError());
    }
    mgr.mFontPointSize = ptsize;
}

TTF_Font* Asset::GetFont()
{
    return Asset::get().mFont;
}

int Asset::GetFontPointSize()
{
    return Asset::get().mFontPointSize;
}

/////////////
// Texture //
/////////////

Texture::Texture(SDL_Renderer* rend, const std::string& path)
{
    SDL_Log("Creating texture %s\n", path.c_str());
    mTexture = IMG_LoadTexture(rend, path.c_str());
    if (!mTexture)
    {
        SDL_Log("Couldn't load texture from file %s: %s\n", path.c_str(), SDL_GetError());
    }
    int w, h;
    int status = SDL_QueryTexture(mTexture, NULL, NULL, &w, &h);
    SDL_Log("status %d\n", status);
}

Texture::Texture(Texture&& other) noexcept
{
    SDL_Log("Moving texture\n");
    mTexture = other.mTexture;

    other.mTexture = nullptr;
}

Texture::~Texture()
{
    SDL_Log("Deleting texture\n");
    if (mTexture != nullptr)
    {
        SDL_Log("Destroying SDL texture\n");
        int status = SDL_QueryTexture(mTexture, NULL, NULL, NULL, NULL);
        if (status < 0)
        {
            SDL_Log("SDL_QueryTexture failed with status %d: %s\n", status, SDL_GetError());
        }

        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

SDL_Texture* Texture::get() const
{
    return mTexture;
}

////////////////
// FONT ATLAS //
////////////////
void Asset::LoadFontAtlas(const std::string& path, int ptsize)
{
    auto& mgr = Asset::get();
    auto fontAtlas = new FontAtlas(mgr.pRenderer, path, ptsize);
    std::string key = std::format("{}:{}", path,ptsize);
    mgr.mFontAtlases.emplace(key, fontAtlas);
}

FontAtlas* Asset::GetFontAtlas(const std::string& path, int ptsize)
{
    std::string key = std::format("{}:{}", path,ptsize);

    auto& mgr = Asset::get();
    auto it = mgr.mFontAtlases.find(key);
    if (it == mgr.mFontAtlases.end())
        LoadFontAtlas(path, ptsize);

    return mgr.mFontAtlases.at(key);
}
