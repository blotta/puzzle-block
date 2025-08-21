#include "particle.hpp"
#include "game.hpp"
#include "log.hpp"
#include <algorithm>
#include <vector>

Particle::Particle(vec2f pos, vec2f vel, float life, SDL_Color startColor, SDL_Color endColor)
    : pos(pos), vel(vel), life(life), maxLife(life), startColor(startColor), endColor(endColor)
{
}

bool Particle::isAlive() const
{
    return life > 0;
}

void Particle::update(float dt)
{
    if (life > 0)
    {
        pos = pos + vel;
        // vel.y += 0.05f; // gravity
        life -= dt;
    }
}

void Particle::draw(int levelX, int levelY) const
{
    if (life > 0)
    {
        SDL_Color c = {
            (Uint8)easings::lerp((int)startColor.r, (int)endColor.r, 1.f - (life / maxLife)),
            (Uint8)easings::lerp((int)startColor.g, (int)endColor.g, 1.f - (life / maxLife)),
            (Uint8)easings::lerp((int)startColor.b, (int)endColor.b, 1.f - (life / maxLife)),
            (Uint8)easings::lerp((int)startColor.a, (int)endColor.a, 1.f - (life / maxLife)),
        };
        Log::debug("life: %f; color: %d, %d, %d, %d", life, c.r, c.g, c.b, c.a);
        SDL_SetRenderDrawColor(Game::GetRenderer(), c.r, c.g, c.b, c.a);
        SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_RenderDrawPointF(Game::GetRenderer(), levelX + pos.x, levelY + pos.y);
    }
}

void ParticleSystem::emit(float x, float y, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (particles.size() < MAX_PARTICLES)
        {
            vec2f pos(x, y);
            vec2f vel = vec2f(randomNeg1to1() * this->vx, randomNeg1to1() * this->vy);
            float life = this->minLife + random01() * (this->maxLife - this->minLife);
            particles.emplace_back(pos, vel, life, this->startColor, this->endColor);
        }
    }
}

void ParticleSystem::emitLine(float x1, float y1, float x2, float y2, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (particles.size() < MAX_PARTICLES)
        {
            float t = random01();
            float x = (1 - t) * x1 + t * x2;
            float y = (1 - t) * y1 + t * y2;
            this->emit(x, y, 1);
        }
    }
}

void ParticleSystem::update(float dt)
{
    for (auto& p : particles)
    {
        p.update(dt);
        p.vel.y += gravity * dt;
    }

    particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle& p) { return !p.isAlive(); }),
                    particles.end());
}

void ParticleSystem::draw(int levelX, int levelY)
{
    for (auto& p : particles)
    {
        p.draw(levelX, levelY);
    }
}
