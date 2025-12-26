#ifndef ASSET_HPP
#define ASSET_HPP

#include "data.hpp"
#include "font.hpp"
#include "texture.hpp"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

using PathId = uint32_t;

class PathTable
{
    std::unordered_map<std::string, PathId> mPathToId;
    std::vector<std::string> mIdToPath;

  public:
    PathId intern(const std::string& path)
    {
        auto it = mPathToId.find(path);
        if (it != mPathToId.end())
            return it->second;

        PathId id = (PathId)mIdToPath.size();
        mIdToPath.push_back(path);
        mPathToId.emplace(mIdToPath.back(), id);
        return id;
    }

    const std::string& resolve(PathId id) const
    {
        return mIdToPath[id];
    }
};


struct FontKey {
    PathId pathId;
    int ptsize;

    bool operator==(const FontKey& other) const noexcept;
};

struct FontKeyHash {
    std::size_t operator()(const FontKey& k) const noexcept;
};

class Asset
{
  public:
    Asset(const Asset&) = delete;
    static void SetRenderer(SDL_Renderer* renderer);

    static const Texture* GetTexture(const std::string& path);
    static void LoadTexture(const std::string& path);

    static Mix_Chunk* GetSound(const std::string& path);
    static void LoadSound(const std::string& path);

    static Mix_Music* GetMusic(const std::string& path);
    static void LoadMusic(const std::string& path);

    static const Font* GetFont(const std::string& path, int ptsize);
    static void LoadFont(const std::string& path, int ptsize);

    static void UnloadAssets();

  private:
    Asset();
    static Asset& get();

    PathTable pathTable;
    SDL_Renderer* pRenderer = nullptr;
    std::unordered_map<std::string, Texture> mTextures;
    std::unordered_map<std::string, Mix_Chunk*> mSounds;
    std::unordered_map<std::string, Mix_Music*> mMusics;
    std::unordered_map<FontKey, Font, FontKeyHash> mFonts;
};

#endif
