#ifndef SCENE_BOOT_HPP
#define SCENE_BOOT_HPP

#include "scene.hpp"

class BootScene : public Scene
{
public:
    BootScene();
    ~BootScene();
    void update(float dt) override;
    void draw() override;
};

#endif