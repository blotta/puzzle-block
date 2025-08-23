#ifndef SCENE_HPP
#define SCENE_HPP

#include "entity.hpp"

enum class Scenes
{
    NONE = 0,
    BOOT,
    SPLASH,
    MAIN_MENU,
    LEVEL_EDIT,
    ISOLEVEL,
    OPTIONS,
    LEVEL_SELECT,
    PAUSE,
    UNITY,
};

class Scene
{
  friend class SceneManager;
  protected:
    EntityManager entities;
  public:
    virtual ~Scene() = 0;
    virtual void init();
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void dispose();
    virtual void onPopReturn();
};

#endif
