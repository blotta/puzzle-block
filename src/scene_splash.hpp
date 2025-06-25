#ifndef SCENE_SPLASH_HPP
#define SCENE_SPLASH_HPP

#include "scene.hpp"
#include "timer.hpp"
#include <string>

class SplashScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;

  private:
    std::string mText;
    Timer mTimer;
    int mIteration = 0;
};

#endif