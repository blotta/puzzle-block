#include <SDL2/SDL.h>

#include "scene_boot.hpp"
#include "game.hpp"

int startX, startY = 0;
Sprite spr = {};

void BootScene::init()
{
    SDL_Log("Loading boot scene\n");
    timer.setDuration(3);
    timer.reset();
    text.Init();

    startX = Game::ScreenWidth()/2;
    startY = Game::ScreenHeight()/2;
    spr = Game::GetSprite(SPR_BLOCK_UP);
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
    
    if (debugMode)
    {
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
            spr = (int)spr.id + 1 > (int)SPR_BLOCK_WIDE_WIDE_60 ? Game::GetSprite(SPR_BLOCK_UP) : Game::GetSprite((SpriteID)((int)spr.id + 1));
        if (Input::JustPressed(SDL_SCANCODE_LEFT))
            spr = (int)spr.id - 1 < (int)SPR_BLOCK_UP ? Game::GetSprite(SPR_BLOCK_WIDE_WIDE_60) : Game::GetSprite((SpriteID)((int)spr.id - 1));

        if (Input::JustPressed(SDL_SCANCODE_L))
            spr.originX -= 1;
        if (Input::JustPressed(SDL_SCANCODE_H))
            spr.originX += 1;
        if (Input::JustPressed(SDL_SCANCODE_K))
            spr.originY += 1;
        if (Input::JustPressed(SDL_SCANCODE_J))
            spr.originY -= 1;

        if (Input::JustPressed(SDL_SCANCODE_R))
        {
            spr = Game::GetSprite(spr.id);
        }
    }
}


void BootScene::draw()
{
    if (!debugMode && !timer.isDone())
        text.Draw(10, 10, "d: debug mode");

    if (debugMode)
    {
        // text.Draw(30, 10, "!\"#$%&'()*+,-./\n0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~");

        char buf[50] = {};
        sprintf(buf, "start: %d %d", startX, startY);
        text.Draw(10, 10, buf);
        sprintf(buf, "origin: %d %d", spr.originX, spr.originY);
        text.Draw(10, 40, buf);

        for (int y = -2; y < 3; y++) {
            for (int x = -2; x < 3; x++) {
                int sx;
                int sy;
                toISO(x, y, 64, 32, &sx, &sy);
                Game::DrawSprite(startX + sx, startY + sy, SPR_FLOOR);
            }
        }

        // mimic Game::DrawSprite()
        SDL_Rect src = {
            spr.tx,
            spr.ty,
            spr.tw,
            spr.th
        };
        SDL_Rect dest = {
            startX - spr.originX,
            startY - spr.originY,
            spr.tw,
            spr.th
        };

        SDL_RenderCopy(Game::GetRenderer(), Asset::GetTexture("assets/images/spritesheet.png")->get(), &src, &dest);

        SDL_SetRenderDrawColor(Game::GetRenderer(), 20, 200, 20, 255);
        SDL_RenderDrawPoint(Game::GetRenderer(), startX, startY);
        SDL_SetRenderDrawColor(Game::GetRenderer(), 200, 200, 20, 255);
        SDL_RenderDrawPoint(Game::GetRenderer(), startX - spr.originX, startY - spr.originY);
    }
}
