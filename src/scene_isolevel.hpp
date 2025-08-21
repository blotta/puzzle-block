#ifndef SCENE_ISOLEVEL_HPP
#define SCENE_ISOLEVEL_HPP

#include "block.hpp"
#include "camera.hpp"
#include "level.hpp"
#include "scene.hpp"

class IsoLevelScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;

  private:
    LevelVisual level;
    BlockVisual block;
    void reset();
    std::string mTitleText;
    bool mLevelCleared = false;
    Animation animLevelCleared;
    AnimationProperty<float> animLevelClearedProp;
};

#endif