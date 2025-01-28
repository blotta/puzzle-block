#include <SDL2/SDL.h>

#include "input_manager.hpp"
#include "scene_boot.hpp"
#include "game.hpp"
#include "util.hpp"

#include "animation.hpp"


enum class BootDebugType
{
    DEBUG_TYPE_INPUT_TEST,
    DEBUG_TYPE_DYNAMIC_TEXT_DRAW,
    DEBUG_TYPE_SPRITE_POSITIONING_UPDATE,
    DEBUG_TYPE_ANIMATION,

    DEBUG_TYPE_COUNT
};

static BootDebugType bootDebugType = BootDebugType::DEBUG_TYPE_INPUT_TEST;

static int startX, startY = 0;
static Sprite spr = {};

static SpriteID up_long_frames[3] = {SPR_BLOCK_UP, SPR_BLOCK_UP_LONG_30, SPR_BLOCK_UP_LONG_60};
static AnimationSprite anim_up_long = {
    .duration = 2.0f,
    .frameCount = 3,
    .frames = up_long_frames,
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

    anim_up_long.start();
}

void BootScene::dispose()
{
    SDL_Log("Unloading boot scene\n");
}

// 0 not pressed
// 1 pressed
// 2 just pressed
// 3 just released
static int spaceKey = 0;
void debug_input_test_update(float dt)
{
    if (Input::Pressed(SDL_SCANCODE_SPACE))
        spaceKey = 1;
    else
        spaceKey = 0;
    if (Input::JustPressed(SDL_SCANCODE_SPACE))
    {
        spaceKey = 2;
        SDL_Log("Just Pressed\n");
    }
    if (Input::JustReleased(SDL_SCANCODE_SPACE))
    {
        spaceKey = 3;
        SDL_Log("Just Released\n");
    }
}

void debug_input_test_draw()
{
    char buf[50] = {};
    sprintf(buf, "space key: %d", spaceKey);
    Game::DrawText(20, 20, buf);
}

// Dynamic Text
void debug_dynamic_text_draw()
{
    Game::DrawText(30, 10, "!\"#$%&'()*+,-./\n0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~");
}

// Sprite positioning
void debug_sprite_positioning_update(float dt)
{
    int dir = 0;
    if (Input::JustPressed(SDL_SCANCODE_RIGHT))
        dir = 1;
    if (Input::JustPressed(SDL_SCANCODE_LEFT))
        dir = -1;
    
    if (dir != 0)
    {
        int offset = (int)SPR_BLOCK_UP;
        int sprCount = (int)SPR_BLOCK_WIDE_UP_60 - offset + 1;
        int sprIdx = offset + cycleIndex((int)spr.id - offset, sprCount, dir);
        spr = Game::GetSprite((SpriteID)sprIdx);
    }

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
    Game::DrawText(10, 10 + 35 * 1, buf);
    sprintf(buf, "spritesheet: %d %d", spr.tx / 128, (spr.ty - 64) / 128);
    Game::DrawText(10, 10 + 35 * 2, buf);

    for (int y = -2; y < 3; y++) {
        for (int x = -2; x < 3; x++) {
            int sx;
            int sy;
            IsoToWorld(x, y, 64, 32, &sx, &sy);
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
            IsoToWorld(x, y, 64, 32, &sx, &sy);
            Game::DrawSprite(startX + sx, startY + sy, SPR_FLOOR);
        }
    }

    // draw animation
    Game::DrawSprite(startX, startY, anim_up_long.tick());
}

void BootScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_D))
    {
        // debug enter
        debugModeActive = true;

        anim_up_long.start();
    }
    
    if (!debugModeActive && timer.isDone())
        loadFirstScene = true;
    
    if (Input::JustPressed(SDL_SCANCODE_RETURN))
        loadFirstScene = true;

    if (loadFirstScene)
        Game::LoadScene(Scenes::SPLASH);
    
    if (debugModeActive)
    {
        if (Input::JustPressed(SDL_SCANCODE_F10))
        {
            bootDebugType = (BootDebugType)cycleIndex((int)bootDebugType, (int)BootDebugType::DEBUG_TYPE_COUNT, -1);
        }
        if (Input::JustPressed(SDL_SCANCODE_F11))
        {
            bootDebugType = (BootDebugType)cycleIndex((int)bootDebugType, (int)BootDebugType::DEBUG_TYPE_COUNT, 1);
        }

        if (bootDebugType == BootDebugType::DEBUG_TYPE_INPUT_TEST)
            debug_input_test_update(dt);
        else if (bootDebugType == BootDebugType::DEBUG_TYPE_SPRITE_POSITIONING_UPDATE)
            debug_sprite_positioning_update(dt);
    }
}


void BootScene::draw()
{
    if (!debugModeActive && !timer.isDone())
        Game::DrawText(10, 10, "d: debug mode");

    if (debugModeActive)
    {
        if (bootDebugType == BootDebugType::DEBUG_TYPE_INPUT_TEST)
            debug_input_test_draw();
        else if (bootDebugType == BootDebugType::DEBUG_TYPE_DYNAMIC_TEXT_DRAW)
            debug_dynamic_text_draw();
        else if (bootDebugType == BootDebugType::DEBUG_TYPE_SPRITE_POSITIONING_UPDATE)
            debug_sprite_positioning_draw();
        else if (bootDebugType == BootDebugType::DEBUG_TYPE_ANIMATION)
            debug_animation_draw();
    }
}
