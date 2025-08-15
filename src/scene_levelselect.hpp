#ifndef SCENE_LEVELSELECT_HPP
#define SCENE_LEVELSELECT_HPP

#include "scene.hpp"
#include "level.hpp"
#include <string>

class LevelSelectScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;

  private:
    int mCursor = 1;
    int mLevelCount = 1;
    int mLvlIdx = 0;
    LevelVisual level;
    std::string mTitle;
    Animation animPanel;
    AnimationProperty<int> animPanelDropHeight;
    Animation animSlide;
    SDL_Texture* mPanelTex;
    bool exiting = false;
};

#endif