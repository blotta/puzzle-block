#ifndef SCENE_BOOT_HPP
#define SCENE_BOOT_HPP

#include "scene.hpp"
#include "timer.hpp"
#include <functional>

enum class BootDebugType
{
    DEBUG_TYPE_INPUT_TEST,
    DEBUG_TYPE_DYNAMIC_TEXT_DRAW,
    DEBUG_TYPE_SPRITE_POSITIONING_UPDATE,
    DEBUG_TYPE_ANIMATION,
    DEBUG_TYPE_ANIMATION_2,
    DEBUG_TYPE_BLOCK,

    DEBUG_TYPE_COUNT
};

class BootScene : public Scene
{
  public:
    BootScene();
    ~BootScene();
    BootScene(BootScene&& other) noexcept;
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;

    void setDebugType(BootDebugType type);

  private:
    Timer timer;
    bool loadFirstScene = false;
    bool debugModeActive = false;
    std::function<void(float)> debug_update_func;
    std::function<void(void)> debug_draw_func;
};

#endif