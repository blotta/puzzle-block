#include <SDL2/SDL.h>

#include "input_manager.hpp"
#include "scene_boot.hpp"
#include "game.hpp"
#include "util.hpp"

#include "animation.hpp"

int startX, startY = 0;
Sprite spr = {};

SpriteID up_long_frames0[3] = {SPR_BLOCK_UP, SPR_BLOCK_UP_LONG_30, SPR_BLOCK_UP_LONG_60};

AnimationSprite anim_up_long0 = {
    .duration = 2.0f,
    .frameCount = 3,
    .frames = up_long_frames0,
    .loop = true,
};

void BootScene::init()
{
    SDL_Log("Loading boot scene\n");
    timer.setDuration(3);
    timer.reset();

    startX = Game::ScreenWidth()/2;
    startY = Game::ScreenHeight()/2;
    spr = Game::GetSprite(SPR_BLOCK_UP);

    anim_up_long0.start();
}

void BootScene::dispose()
{
    SDL_Log("Unloading boot scene\n");
}

// Dynamic Text
void debug_dynamic_text_draw()
{
    Game::DrawText(30, 10, "!\"#$%&'()*+,-./\n0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~");
}

// Sprite positioning
void debug_sprite_positioning_update(float dt)
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

void debug_sprite_positioning_draw()
{
    char buf[50] = {};
    sprintf(buf, "start: %d %d", startX, startY);
    Game::DrawText(10, 10, buf);
    sprintf(buf, "origin: %d %d", spr.originX, spr.originY);
    Game::DrawText(10, 45, buf);

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

// Animation test
void debug_animation_draw()
{
    // draw floors
    for (int y = -2; y < 3; y++) {
        for (int x = -2; x < 3; x++) {
            int sx;
            int sy;
            toISO(x, y, 64, 32, &sx, &sy);
            Game::DrawSprite(startX + sx, startY + sy, SPR_FLOOR);
        }
    }

    // draw animation
    Game::DrawSprite(startX, startY, anim_up_long0.tick());
}

void BootScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_D))
    {
        // debug enter
        debugMode = true;

        anim_up_long0.start();
    }
    
    if (!debugMode && timer.isDone())
        loadFirstScene = true;
    
    if (Input::JustPressed(SDL_SCANCODE_RETURN))
        loadFirstScene = true;

    if (loadFirstScene)
        Game::LoadScene(Scenes::SPLASH);
    
    if (debugMode)
    {
        //debug_sprite_positioning_update(dt);
    }
}


void BootScene::draw()
{
    if (!debugMode && !timer.isDone())
        Game::DrawText(10, 10, "d: debug mode");

    if (debugMode)
    {
        // debug_sprite_positioning_draw();
        debug_animation_draw();
    }
}
