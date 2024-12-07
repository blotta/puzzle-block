#ifndef SCENE_LEVEL_HPP
#define SCENE_LEVEL_HPP

#include "scene.hpp"
#include "level.hpp"
#include "block.hpp"

class LevelEditScene : public Scene
{
public:
    LevelEditScene();
    ~LevelEditScene();
    void update(float dt) override;
    void draw() override;
    void input() override;
private:
    int timer;
    Level level;
    int lvlIdx;
    Block block;
    int offsetX = 100;
    int offsetY = 100;
    int cellSize = 64;
    vec2 moveDir;
    vec2 mousePos;
    vec2 mouseGridPos;
    bool saved = true;
    void reset();
    void resize();
};

#endif