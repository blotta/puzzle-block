#ifndef SCENE_SPLASH_HPP
#define SCENE_SPLASH_HPP

#include "animation.hpp"
#include "scene.hpp"
#include "timer.hpp"
#include <string>

class SplashScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    void dispose() override;

  private:
    Timer mTimer;
    bool splashDone;

    Animation splashAnim;
    AnimationProperty<vec2> animB;
    AnimationProperty<vec2> animL;
    AnimationProperty<vec2> animT;
    AnimationProperty<vec2> animGames;
    AnimationProperty<float> animShake;
    AnimationProperty<vec2> animSquare;

    Animation fadeAnim;
    AnimationProperty<int> animFade;
};

#endif