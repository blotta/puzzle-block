#ifndef SCENE_OPTIONS_HPP
#define SCENE_OPTIONS_HPP

#include "animation.hpp"
#include "component_gui.hpp"
#include "scene.hpp"
#include <SDL.h>

enum class OptionsState
{
    IDLE,
    EXITING,
};

class OptionsScene : public Scene
{
  public:
    void preload() override;
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    int mPanelWidth = 400;
    int mPanelHeight = 300;
    OptionsState mState = OptionsState::IDLE;
    GuiComponent* gui;
    Panel* panel;
    Cursor* cursor;
    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
};

#endif
