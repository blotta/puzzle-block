#ifndef SCENE_BOOT_HPP
#define SCENE_BOOT_HPP

#include "scene.hpp"

#include "render_text.hpp"

class BootScene : public Scene
{
public:
    BootScene(Game* game);
    ~BootScene();
    void update() override;
    void draw() override;
    void input() override;
private:
    float timer = 0;
    RenderText mText;
};

#endif