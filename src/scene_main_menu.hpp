#ifndef SCENE_MAIN_MENU_HPP
#define SCENE_MAIN_MENU_HPP

#include "block.hpp"
#include "camera.hpp"
#include "level.hpp"
#include "scene.hpp"

class MainMenuScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;
    void onPopReturn() override;

  private:
    LevelVisual level;
    BlockVisual block;
    Camera camera;
    int cellSize = 64;
};

#endif
