#include "scene_boot.hpp"

BootScene::BootScene(Game* game)
    : Scene(game)
{
    SDL_Log("Loading boot scene\n");
}

BootScene::~BootScene()
{
    SDL_Log("Unloading boot scene\n");
}

void BootScene::input()
{
}

void BootScene::update(float dt)
{
    game->loadScene(Scenes::SPLASH);
}

void BootScene::draw()
{
}
