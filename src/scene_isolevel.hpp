#ifndef SCENE_ISOLEVEL_HPP
#define SCENE_ISOLEVEL_HPP

#include "scene.hpp"
#include "level.hpp"
#include "block.hpp"

class IsoLevelScene : public Scene
{
public:
    IsoLevelScene(Game* game);
    ~IsoLevelScene();
    void update(float dt) override;
    void draw() override;
    void input() override;
private:
    int timer;
    Level level;
    Block block;
    int offsetX = 100;
    int offsetY = 100;
    int cellSize = 64;
    vec2 moveDir;
    vec2 mousePos;
    bool playerLost;
    void reset();
};

#endif