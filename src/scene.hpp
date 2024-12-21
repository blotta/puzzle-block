#ifndef SCENE_HPP
#define SCENE_HPP

enum class Scenes
{
    NONE = 0,
    BOOT,
    SPLASH,
    LEVEL_EDIT,
    ISOLEVEL,
};


class Scene
{
public:
    virtual ~Scene() = 0;
    virtual void init();
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void dispose();
};

#endif