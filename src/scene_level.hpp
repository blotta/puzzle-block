#ifndef SCENE_LEVEL_HPP
#define SCENE_LEVEL_HPP

#include "scene.hpp"

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
};

#endif