#include "scene_boot.hpp"

BootScene::BootScene(Game* game)
    : Scene(game)
    , mText(game->getFont())
{
    SDL_Log("Loading boot scene\n");
    timer = 0;
}

BootScene::~BootScene()
{
    SDL_Log("Unloading boot scene\n");
}

void BootScene::input()
{
}

void BootScene::update()
{
    timer += 1;
    if (timer >= 60 * 5)
    {
        game->loadScene(Scenes::SPLASH);
    }

    if (timer >= 60 * 3)
        mText.setText("BLT Games");
    else if (timer >= 60 * 2)
        mText.setText("okk");
    else if (timer >= 60 * 1)
        mText.setText("ok");
}

void BootScene::draw()
{
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pRenderer);

    mText.draw(
        pRenderer,
        game->ScreenWidth / 2 - 100,
        game->ScreenHeight / 2 - 100
    );

    SDL_RenderPresent(pRenderer);
}
