#ifndef SCENE_SPLASH_HPP
#define SCENE_SPLASH_HPP

#include "scene.hpp"

class SplashScene : public Scene
{
public:
    SplashScene(Game* game);
    ~SplashScene();
    void update() override;
    void draw() override;
    void input() override;
private:
    SDL_Texture* pSplashTexture;
};

#endif