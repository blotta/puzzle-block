#include <SDL2/SDL.h>

#include "scene_boot.hpp"
#include "game.hpp"

void BootScene::init()
{
    SDL_Log("Loading boot scene\n");
    timer.setDuration(3);
    timer.reset();
    text.Init();
}

void BootScene::dispose()
{
    SDL_Log("Unloading boot scene\n");
    text.Destroy();
}

void BootScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_D))
        debugMode = true;
    
    if (!debugMode && timer.isDone())
        loadFirstScene = true;
    
    if (Input::JustPressed(SDL_SCANCODE_RETURN))
        loadFirstScene = true;

    if (loadFirstScene)
        Game::LoadScene(Scenes::SPLASH);
}

void BootScene::draw()
{
    if (!debugMode && !timer.isDone())
        text.Draw(10, 10, "d: debug mode");

    if (debugMode)
    {
        text.Draw(30, 10, "!\"#$%&'()*+,-./\n0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~");
    }
}
