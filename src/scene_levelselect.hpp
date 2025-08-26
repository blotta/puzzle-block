#ifndef SCENE_LEVELSELECT_HPP
#define SCENE_LEVELSELECT_HPP

#include "scene.hpp"
#include "level.hpp"
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
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    int mLevelCount = 1;
    int mLvlIdx = 0;
    LevelVisual level;
    LevelVisual levelAux;
    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
    Animation animLevelSlide;
    AnimationProperty<int> animLevelSlideLeft;
    SDL_Texture* mPanelTex;
    LevelSelectState mState = LevelSelectState::IDLE;
};

#endif