#ifndef SCENE_HPP
#define SCENE_HPP

#include "game.hpp"

enum class Scenes
{
    NONE = 0,
    BOOT,
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
protected:
};

#endif