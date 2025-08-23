#ifndef SCENE_UNITY_HPP
#define SCENE_UNITY_HPP

#include "animation.hpp"
#include "scene.hpp"
#include "timer.hpp"
#include "entity.hpp"
#include <string>

class UnityScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;
};

#endif