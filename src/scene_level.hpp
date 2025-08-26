#ifndef SCENE_LEVEL_HPP
#define SCENE_LEVEL_HPP

#include "block.hpp"
#include "level.hpp"
#include "scene.hpp"
#include "component_level.hpp"

class LevelScene : public Scene
{
  public:
    void preload() override;
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    LevelComponent* lc;
    BlockComponent* bc;
    std::string title;
};

#endif