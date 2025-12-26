#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "vmath.hpp"
#include <SDL.h>
#include <vector>

#define MAX_PARTICLES 500

struct Particle
{
    vec2f pos;
    vec2f vel;
    float life;
    float maxLife;
    SDL_Color startColor;
    SDL_Color endColor;

    Particle(vec2f pos, vec2f vel, float life, SDL_Color startColor, SDL_Color endColor);

    bool isAlive() const;

    void update(float dt);

    void draw(int levelX, int levelY) const;
};

class ParticleSystem
{
    std::vector<Particle> particles;

  public:
    float vx = 5.f;
    float vy = 5.f;
    float gravity = 5.f;
    float minLife = 1.f;
    float maxLife = 2.f;
    SDL_Color startColor = {255, 255, 255, 255};
    SDL_Color endColor = {255, 255, 255, 255};
    void emit(float x, float y, int count = 10);
    void emitLine(float x1, float y1, float x2, float y2, int count = 10);
    void update(float dt);
    void draw(int levelX, int levelY);
};

#endif
