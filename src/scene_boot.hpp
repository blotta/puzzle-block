#ifndef SCENE_BOOT_HPP
#define SCENE_BOOT_HPP

#include "scene.hpp"
#include "timer.hpp"

class BootScene : public Scene
{
public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;
private:
    Timer timer;
    bool loadFirstScene;
    bool debugModeActive;
};

#endif