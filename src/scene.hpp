#ifndef SCENE_HPP
#define SCENE_HPP

#include "game.hpp"

enum class Scenes
{
    BOOT = 0,
    SPLASH,
    LEVEL_EDIT,
    ISOLEVEL,
};

class Game;

class Scene
{
public:
    Scene();
    virtual ~Scene() {}
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void input() = 0;
protected:
};

#endif