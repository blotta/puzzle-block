#ifndef SCENE_LEVELEDITNEW_HPP
#define SCENE_LEVELEDITNEW_HPP

#include "block.hpp"
#include "level.hpp"
#include "scene.hpp"
#include "component_leveledit.hpp"

enum class LevelEditMode
{
  NORMAL,
  SWITCH
};

class LevelEditNewScene : public Scene
{
  public:
    void preload() override;
    void init() override;
    void update(float dt) override;
    void draw() override;
    void drawGUI() override;

  private:
    LevelEditComponent* lc;
    LevelSwitch tmpSwitch = {};
    vec2 mouseIsoPos;
    LevelEditMode mMode = LevelEditMode::NORMAL;
};

#endif