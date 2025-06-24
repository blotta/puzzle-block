#include "scene_splash.hpp"
#include "game.hpp"
#include "input_manager.hpp"

void SplashScene::init()
{
    SDL_Log("Loading splash scene\n");
    mText.clearText();
    mText.hAlign = 1; // center
    mTimer.setDuration(0);
    mTimer.reset();
}

void SplashScene::dispose()
{
    SDL_Log("Unloading splash scene\n");
}

void SplashScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_RETURN))
    {
        Game::LoadScene(Scenes::ISOLEVEL);
        return;
    }

    if (mTimer.isDone())
    {
        mIteration += 1;
        mTimer.reset();
        mTimer.setDuration(1.0);

        switch (mIteration)
        {
        case 1:
            mText.setText("B");
            break;
        case 2:
            mText.setText("BL");
            break;
        case 3:
            mText.setText("BLT");
            break;
        case 4:
            mText.setText("BLT GAMES");
            break;
        default:
            Game::LoadScene(Scenes::ISOLEVEL);
            break;
        }
    }
}

void SplashScene::draw()
{
    mText.draw(Game::ScreenWidth() / 2, Game::ScreenHeight() / 2);
}
