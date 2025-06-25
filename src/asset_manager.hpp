#ifndef ASSET_HPP
#define ASSET_HPP

#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "data.hpp"
#include "font.hpp"
#include "texture.hpp"


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

    SDL_Renderer* pRenderer = nullptr;
    std::unordered_map<std::string, Texture> mTextures;
    std::unordered_map<std::string, Mix_Chunk*> mSounds;
    std::unordered_map<std::string, Mix_Music*> mMusics;
    std::unordered_map<std::string, Font> mFonts;
};

#endif
