#ifndef SCENE_HPP
#define SCENE_HPP

#include "game.hpp"

enum class Scenes
{
    BOOT = 0,
    SPLASH,
    LEVEL,
};

class Game;

class Scene
{
public:
    Scene(Game* game);
    virtual ~Scene() {}
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void input() = 0;
protected:
    Game* game;
    SDL_Renderer* pRenderer;
};

#endif