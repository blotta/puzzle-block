#include <SDL2/SDL.h>

#include "scene_boot.hpp"
#include "game.hpp"

void BootScene::init()
{
    SDL_Log("Loading boot scene\n");
}

void BootScene::dispose()
{
    SDL_Log("Unloading boot scene\n");
}

void BootScene::update(float dt)
{
    Game::LoadScene(Scenes::ISOLEVEL);
}

void BootScene::draw()
{
}
