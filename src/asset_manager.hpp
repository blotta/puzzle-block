#ifndef ASSET_HPP
#define ASSET_HPP

#include <vector>
#include <string>
#include <span>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "data.hpp"

class Texture
{
public:
    Texture(SDL_Renderer* rend, const std::string& path);
    Texture(const Texture& other) = delete; // disallows copying
    // Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other) noexcept; // move constructor
    ~Texture();
    SDL_Texture* get() const;
private:
    SDL_Texture* mTexture = NULL;
};


class Asset
{
public:
    Asset(const Asset&) = delete;
    static void SetRenderer(SDL_Renderer* renderer);
    static const Texture* GetTexture(const std::string& path);
    static void LoadTexture(const std::string& path);

    static void LoadFont(const std::string& path, int ptsize);
    static TTF_Font* GetFont();

    static void UnloadAssets();
private:
    Asset();
    static Asset& get();

    SDL_Renderer* pRenderer = nullptr;
    std::unordered_map<std::string, Texture> mTextures;
    TTF_Font* mFont = nullptr;
};

#endif
