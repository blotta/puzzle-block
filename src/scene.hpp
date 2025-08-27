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
    LEVEL,
    LEVEL_EDIT_NEW,
};

class Scene
{
  friend class SceneManager;
  protected:
    EntityManager entities;
  public:
    virtual ~Scene() = 0;
    virtual void preload(); // entity setup
    virtual void init(); // after entity init
    virtual void update(float dt);
    virtual void draw(); // camera offset enabled
    virtual void drawGUI(); // camera offset disabled
    virtual void dispose();
    virtual void onPopReturn();
};

#endif
