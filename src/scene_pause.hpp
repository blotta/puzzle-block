#ifndef SCENE_PAUSE_HPP
#define SCENE_PAUSE_HPP

#include "component_gui.hpp"
#include "level.hpp"
#include "scene.hpp"
#include <string>

enum class PauseState
{
    IDLE,
    EXITING
};

class PauseScene : public Scene
{
  public:
    void preload() override;
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    int mPanelWidth = 250;
    int mPanelHeight = 300;
    Panel* panel;
    Cursor* cursor;
    PauseState mState = PauseState::IDLE;
    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
};

#endif