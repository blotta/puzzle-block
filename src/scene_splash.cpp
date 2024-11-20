#include "scene_splash.hpp"

SplashScene::SplashScene(Game* game) : Scene(game)
{
    SDL_Log("Loading splash scene\n");
    pSplashTexture = game->getTexture("splash");
}

SplashScene::~SplashScene()
{
    SDL_Log("Unloading splash scene\n");
}

void SplashScene::input()
{
}

void SplashScene::update()
{
}

void SplashScene::draw()
{
    SDL_SetRenderDrawColor(pRenderer, 0, 100, 50, 255);
    SDL_RenderClear(pRenderer);

    SDL_Rect rect = {
        20, 20, 50, 50};
    SDL_SetRenderDrawColor(pRenderer, 20, 80, 200, 255);
    SDL_RenderFillRect(pRenderer, &rect);

    SDL_Rect splash = {
        300, 300, 400, 200};
    SDL_RenderCopy(pRenderer, pSplashTexture, NULL, &splash);

    SDL_RenderPresent(pRenderer);
}
