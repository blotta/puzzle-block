#ifndef SCENE_BOOT_HPP
#define SCENE_BOOT_HPP

#include "scene.hpp"

#include "render_text.hpp"

class BootScene : public Scene
{
public:
    BootScene(Game* game);
    ~BootScene();
    void update(float dt) override;
    void draw() override;
    void input() override;
};

#endif