#ifndef SCENE_OPTIONS_HPP
#define SCENE_OPTIONS_HPP

#include "animation.hpp"
#include "scene.hpp"
#include <SDL2/SDL.h>

enum class OptionsState
{
    IDLE,
    EXITING,
};

class OptionsScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    int mCursor = 1;
    int mLines = 2; // sfx, music
    int mPanelWidth = 600;
    int mPanelHeight = 400;
    OptionsState mState = OptionsState::IDLE;
    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
    SDL_Texture* mPanelTex;
};

#endif