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
};

class LevelSelectWidget;

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
    LevelSelectWidget* levelSelectWidget;

    int mPanelWidth = 850;
    int mPanelHeight = 500;

    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
    LevelSelectState mState = LevelSelectState::IDLE;
};

#endif