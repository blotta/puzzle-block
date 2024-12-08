#ifndef SCENE_SPLASH_HPP
#define SCENE_SPLASH_HPP

#include "scene.hpp"
#include "text.hpp"

class SplashScene : public Scene
{
public:
    SplashScene();
    ~SplashScene();
    void update(float dt) override;
    void draw() override;
private:
    const Texture* pSplashTexture;
    StaticText mText;
    float mTimer;
};

#endif