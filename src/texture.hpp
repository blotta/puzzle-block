#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL.h>
#include <string>

class Texture
{
  public:
    Texture(SDL_Renderer* rend, const std::string& path);
    Texture(const Texture& other) = delete; // disallows copying
    // Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other) noexcept; // move constructor
    ~Texture();
    SDL_Texture* get() const;
    void drawTile(const SDL_Rect* src, const SDL_Rect* dest) const;

    std::string path;
  private:
    SDL_Renderer* pRenderer = nullptr;
    SDL_Texture* mTexture = nullptr;
};

#endif
