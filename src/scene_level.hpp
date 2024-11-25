#ifndef SCENE_LEVEL_HPP
#define SCENE_LEVEL_HPP

#include "scene.hpp"
#include "level.hpp"

class LevelScene : public Scene
{
public:
    LevelScene(Game* game);
    ~LevelScene();
    void update(float dt) override;
    void draw() override;
    void input() override;
private:
    int timer;
    Level level;
    int offsetX = 100;
    int offsetY = 100;
    int cellSize = 64;
};

#endif