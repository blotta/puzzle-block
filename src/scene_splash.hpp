#ifndef SCENE_SPLASH_HPP
#define SCENE_SPLASH_HPP

#include "scene.hpp"
#include "render_text.hpp"

class SplashScene : public Scene
{
public:
    SplashScene(Game* game);
    ~SplashScene();
    void update(float dt) override;
    void draw() override;
    void input() override;
private:
    const Texture* pSplashTexture;
    RenderText mText;
    float mTimer;
};

#endif