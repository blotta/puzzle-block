#include "scene_boot.hpp"

BootScene::BootScene()
{
    SDL_Log("Loading boot scene\n");
}

BootScene::~BootScene()
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
