#include "scene_splash.hpp"

SplashScene::SplashScene(Game* game)
    : Scene(game)
    , mText(game->getFont())
{
    SDL_Log("Loading splash scene\n");
    pSplashTexture = game->getTexture("splash");
    mTimer = 0;
    mText.clearText();
}

SplashScene::~SplashScene()
{
    SDL_Log("Unloading splash scene\n");
}

void SplashScene::input()
{
}

void SplashScene::update(float dt)
{
    mTimer += dt;
    if (mTimer > 3)
    {
        mText.setText("BLT Games");
    }
    else if (mTimer > 2)
    {
        mText.setText("BLT");
    }
    else if (mTimer > 1.5)
    {
        mText.setText("BL");
    }
    else if (mTimer > 1)
    {
        mText.setText("B");
    }

    if (mTimer > 4)
    {
        game->loadScene(Scenes::LEVEL);
    }
}

void SplashScene::draw()
{
    mText.draw(pRenderer, game->ScreenWidth/2 - mText.getWidth()/2, game->ScreenHeight/2 - mText.getHeight()/2);
}
