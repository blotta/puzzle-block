#include "asset_manager.hpp"
#include <SDL2/SDL_image.h>
#include <format>
#include "log.hpp"

Asset& Asset::get()
{
    static Asset instance;
    return instance;
}

Asset::Asset()
{
    Log::debug("Asset initialized\n");
}

void Asset::SetRenderer(SDL_Renderer* renderer)
{
    Asset::get().pRenderer = renderer;
}

void Asset::UnloadAssets()
{
    auto& mgr = Asset::get();
    mgr.mTextures.clear();
    mgr.mFonts.clear();

    for (const auto& [path, sfx] : mgr.mSounds)
    {
        Mix_FreeChunk(sfx);
    }

    for (const auto& [path, sfx] : mgr.mMusics)
    {
        Mix_FreeMusic(sfx);
    }
}

/////////////
// Texture //
/////////////

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

///////////////////
// SOUND & MUSIC //
///////////////////

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

//////////
// FONT //
//////////

const Font* Asset::GetFont(const std::string& path, int ptsize)
{
    std::string key = std::format("{}:{}", path, ptsize);

    auto& mgr = Asset::get();
    auto it = mgr.mFonts.find(key);
    if (it == mgr.mFonts.end())
        LoadFont(path, ptsize);

    return &mgr.mFonts.at(key);
}

void Asset::LoadFont(const std::string& path, int ptsize)
{
    std::string key = std::format("{}:{}", path, ptsize);

    auto& mgr = Asset::get();
    Font font(mgr.pRenderer, path, ptsize);
    mgr.mFonts.emplace(key, std::move(font));
}
