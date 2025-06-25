#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL.h>
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

  private:
    SDL_Texture* mTexture = NULL;
};

#endif