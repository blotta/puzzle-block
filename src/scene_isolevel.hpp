#ifndef SCENE_ISOLEVEL_HPP
#define SCENE_ISOLEVEL_HPP

#include "text.hpp"
#include "scene.hpp"
#include "level.hpp"
#include "block.hpp"
#include "camera.hpp"

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
    StaticText mTitleText;
    bool mLevelCleared = false;
    StaticText mLevelClearedText;
    Timer mLevelClearedTimer;
};

#endif