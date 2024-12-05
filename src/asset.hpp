#ifndef ASSET_HPP
#define ASSET_HPP

#include <vector>
#include <string>
#include <span>
#include <unordered_map>

#include <SDL2/SDL.h>
#include "sprite.hpp"

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




// class Atlas
// {
// public:
//     Atlas();
//     Atlas(Texture& texture);
//     // Atlas(const std::string& texturePath);
//     // Atlas(const std::string& texturePath, std::span<IsoSprite> data);
//     void add(IsoSprite data);
// private:
//     Texture& mTexture;
//     std::vector<IsoSprite> mData;
// };




class AssetManager
{
public:
    AssetManager(SDL_Renderer* renderer);
    ~AssetManager();

    void unloadAssets();

    const Texture* getTexture(const std::string& path);
    void setRenderer(SDL_Renderer* renderer);

    // void createAtlas(const std::string& name, const std::string& texturePath);
    // void addToAtlas(const std::string& name, IsoSprite data);

private:
    SDL_Renderer* pRenderer;
    std::unordered_map<std::string, Texture> mTextures;
    // std::unordered_map<std::string, Atlas> mAtlases;
    void loadTexture(const std::string& path);
};
// extern std::vector<IsoSprite> ASSET_DEFS;

#endif
