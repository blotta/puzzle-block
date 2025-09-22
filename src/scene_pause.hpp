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
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    int mPanelWidth = 600;
    int mPanelHeight = 400;
    int mCursor = 1;
    int mLines = 4; // resume, options, mainmenu, exit
    Panel* panel;
    Cursor* cursor;
    PauseState mState = PauseState::IDLE;
    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
    SDL_Texture* mPanelTex;
};

#endif