#ifndef SCENE_LEVELSELECT_HPP
#define SCENE_LEVELSELECT_HPP

#include "component_gui.hpp"
#include "level.hpp"
#include "scene.hpp"
#include <string>

enum class LevelSelectState
{
    IDLE,
    EXITING,
    SLIDING_LEVEL_LEFT,
    SLIDING_LEVEL_RIGHT,
};

class LevelSelectScene : public Scene
{
  public:
    void preload() override;
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    GuiComponent* gui;
    Panel* panel;
    Cursor* cursor;
    Panel* levelListCont;

    int mPanelWidth = 850;
    int mPanelHeight = 500;

    int mLevelCount = 1;
    int mLvlIdx = 0;
    LevelVisual level;
    LevelVisual levelAux;
    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
    Animation animLevelSlide;
    AnimationProperty<int> animLevelSlideLeft;
    LevelSelectState mState = LevelSelectState::IDLE;
};

#endif