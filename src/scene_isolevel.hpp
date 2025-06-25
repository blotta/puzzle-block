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
    Level level;
    Block block;
    Camera camera;
    int cellSize = 64;
    vec2 moveDir;
    vec2 mousePos;
    void reset();
    std::string mTitleText;
    bool mLevelCleared = false;
    Timer mLevelClearedTimer;
};

#endif