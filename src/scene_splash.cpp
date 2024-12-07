#include "scene_splash.hpp"
#include "input_manager.hpp"

SplashScene::SplashScene()
{
    SDL_Log("Loading splash scene\n");
    pSplashTexture = Asset::GetTexture("assets/images/splash.png");
    mTimer = 0;
    mText.clearText();
}

SplashScene::~SplashScene()
{
    SDL_Log("Unloading splash scene\n");
}

void SplashScene::input()
{
    if (Input::JustPressed(SDL_SCANCODE_RETURN))
    {
        Game::LoadScene(Scenes::LEVEL_EDIT);
    }
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
        Game::LoadScene(Scenes::LEVEL_EDIT);
    }
}

void SplashScene::draw()
{
    mText.draw(Game::GetRenderer(), Game::ScreenWidth()/2 - mText.getWidth()/2, Game::ScreenHeight()/2 - mText.getHeight()/2);
}
