#ifndef SCENE_ISOLEVEL_HPP
#define SCENE_ISOLEVEL_HPP

#include "text.hpp"
#include "scene.hpp"
#include "level.hpp"
#include "block.hpp"

class IsoLevelScene : public Scene
{
public:
    IsoLevelScene();
    ~IsoLevelScene();
    void update(float dt) override;
    void draw() override;
private:
    Level level;
    Block block;
    int offsetX = 100;
    int offsetY = 100;
    int cellSize = 64;
    int lvlIdx;
    vec2 moveDir;
    vec2 mousePos;
    void reset();
    StaticText mTitleText;
    bool mLevelCleared = false;
    StaticText mLevelClearedText;
    Timer mLevelClearedTimer;
};

#endif