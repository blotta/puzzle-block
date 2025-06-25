#include "scene_splash.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"

void SplashScene::init()
{
    Log::info("Loading splash scene\n");
    
    mTimer.setDuration(0);
    mTimer.reset();

    Game::SetFontSize(32);
}

void SplashScene::dispose()
{
    Log::info("Unloading splash scene\n");
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
            mText = "B";
            break;
        case 2:
            mText = "BL";
            break;
        case 3:
            mText = "BLT";
            break;
        case 4:
            mText = "BLT GAMES";
            break;
        default:
            Game::LoadScene(Scenes::ISOLEVEL);
            break;
        }
    }
}

void SplashScene::draw()
{
    Game::Text(Game::ScreenWidth() / 2, Game::ScreenHeight() / 2, mText, {255, 255, 255, 255}, TextAlign::CENTER);
}
