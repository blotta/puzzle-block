#include <SDL2/SDL.h>

#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include "scene_boot.hpp"
#include "util.hpp"

#include "animation.hpp"
#include <format>

static BootDebugType bootDebugType = BootDebugType::DEBUG_TYPE_INPUT_TEST;

void debug_input_test_update(float dt);
void debug_input_test_draw();

void debug_dynamic_text_draw();

void debug_sprite_positioning_update(float dt);
void debug_sprite_positioning_draw();

void debug_animation_draw();

void debug_animation2_update(float dt);
void debug_animation2_draw();

inline const char* toString(BootDebugType state)
{
    switch (state)
    {
    case BootDebugType::DEBUG_TYPE_INPUT_TEST:
        return "Input Test";
    case BootDebugType::DEBUG_TYPE_DYNAMIC_TEXT_DRAW:
        return "Dynamic Text Draw";
    case BootDebugType::DEBUG_TYPE_SPRITE_POSITIONING_UPDATE:
        return "Sprite Positioning Update";
    case BootDebugType::DEBUG_TYPE_ANIMATION:
        return "Animation";
    case BootDebugType::DEBUG_TYPE_ANIMATION_2:
        return "Animation 2";
    default:
        return "UNKNOWN";
    }
}

void noop_update(float dt)
{
}
void noop_draw()
{
}

void BootScene::setDebugType(BootDebugType type)
{
    this->debug_update_func = noop_update;
    this->debug_draw_func = noop_draw;

    switch (type)
    {
    case BootDebugType::DEBUG_TYPE_INPUT_TEST:
        this->debug_update_func = debug_input_test_update;
        this->debug_draw_func = debug_input_test_draw;
        break;
    case BootDebugType::DEBUG_TYPE_DYNAMIC_TEXT_DRAW:
        this->debug_draw_func = debug_dynamic_text_draw;
        break;
    case BootDebugType::DEBUG_TYPE_SPRITE_POSITIONING_UPDATE:
        this->debug_update_func = debug_sprite_positioning_update;
        this->debug_draw_func = debug_sprite_positioning_draw;
        break;
    case BootDebugType::DEBUG_TYPE_ANIMATION:
        this->debug_draw_func = debug_animation_draw;
        break;
    case BootDebugType::DEBUG_TYPE_ANIMATION_2:
        this->debug_update_func = debug_animation2_update;
        this->debug_draw_func = debug_animation2_draw;
        break;
    default:
        break;
    }
}

void BootScene::init()
{
    Log::info("Loading boot scene\n");
    timer.setDuration(3);
    timer.reset();

    Game::SetFontSize(20);

    setDebugType(bootDebugType);
}

void BootScene::dispose()
{
    Log::info("Unloading boot scene\n");
}

void BootScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_D))
    {
        // debug enter
        debugModeActive = true;
    }

    if (!debugModeActive && timer.isDone())
        loadFirstScene = true;

    if (Input::JustPressed(SDL_SCANCODE_RETURN))
        loadFirstScene = true;

    if (loadFirstScene)
        Game::LoadScene(Scenes::SPLASH);

    if (debugModeActive)
    {
        if (Input::JustPressed(SDL_SCANCODE_F9))
        {
            bootDebugType = (BootDebugType)cycleIndex((int)bootDebugType, (int)BootDebugType::DEBUG_TYPE_COUNT, -1);
            setDebugType(bootDebugType);
        }
        if (Input::JustPressed(SDL_SCANCODE_F10))
        {
            bootDebugType = (BootDebugType)cycleIndex((int)bootDebugType, (int)BootDebugType::DEBUG_TYPE_COUNT, 1);
            setDebugType(bootDebugType);
        }

        this->debug_update_func(dt);
    }
}

void BootScene::draw()
{
    if (!debugModeActive && !timer.isDone())
        Game::Text(10, 10, "d: debug mode");

    if (debugModeActive)
    {
        this->debug_draw_func();

        Game::Text(Game::ScreenWidth() - 10, 10,
                   std::format("[{}]\n"
                               "Cycle: F9-F10",
                               toString(bootDebugType)),
                   {.align = TextAlign::RIGHT});
    }
}

////////////////
// Input Test //
////////////////

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
        Log::debug("Just Pressed\n");
    }
    if (Input::JustReleased(SDL_SCANCODE_SPACE))
    {
        spaceKey = 3;
        Log::debug("Just Released\n");
    }
}

void debug_input_test_draw()
{
    Game::Text(10, 10, std::format("space key: {}", spaceKey));
}

//////////////////
// Dynamic Text //
//////////////////
void debug_dynamic_text_draw()
{
    static const char* text =
        "!\"#$%&'()*+,-./\n0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~";

    Game::Text(10, 10, text);
    Game::Text(Game::ScreenWidth() / 2, Game::ScreenHeight() / 2, text,
               {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
    Game::Text(Game::ScreenWidth() - 10, Game::ScreenHeight() - 10, text,
               {.align = TextAlign::RIGHT, .valign = VerticalAlign::BOTTOM});
}

////////////////////////
// Sprite positioning //
////////////////////////
static bool debug_sprite_positioning_initted = false;
static int startX, startY = 0;
static Sprite spr = {};

void debug_sprite_positioning_update(float dt)
{
    if (!debug_sprite_positioning_initted)
    {
        debug_sprite_positioning_initted = true;
        startX = Game::ScreenWidth() / 2;
        startY = Game::ScreenHeight() / 2;
        spr = Game::GetSprite(SPR_BLOCK_UP);
    }

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
    Game::Text(10, 10,
               std::format("start: {} {}\n"
                           "origin: {} {}\n"
                           "spritesheet: {} {}",
                           startX, startY, spr.originX, spr.originY, spr.tx / 128, (spr.ty - 64) / 128));

    Game::Text(10, Game::ScreenHeight() - 10,
               "change sprite: LEFT/RIGHT\n"
               "move origin: HJKL\n"
               "reset: R",
               {.align = TextAlign::LEFT, .valign = VerticalAlign::BOTTOM});

    for (int y = -2; y < 3; y++)
    {
        for (int x = -2; x < 3; x++)
        {
            int sx;
            int sy;
            IsoToWorld(x, y, 64, 32, &sx, &sy);
            Game::DrawSprite(startX + sx, startY + sy, SPR_FLOOR);
        }
    }

    // mimic Game::DrawSprite()
    SDL_Rect src = {spr.tx, spr.ty, spr.tw, spr.th};
    SDL_Rect dest = {startX - spr.originX, startY - spr.originY, spr.tw, spr.th};

    SDL_RenderCopy(Game::GetRenderer(), Asset::GetTexture("assets/images/spritesheet.png")->get(), &src, &dest);

    SDL_SetRenderDrawColor(Game::GetRenderer(), 20, 200, 20, 255);
    SDL_RenderDrawPoint(Game::GetRenderer(), startX, startY);
    SDL_SetRenderDrawColor(Game::GetRenderer(), 200, 200, 20, 255);
    SDL_RenderDrawPoint(Game::GetRenderer(), startX - spr.originX, startY - spr.originY);
}

////////////////////
// Animation test //
////////////////////
static bool debug_animation_initted = false;
static SpriteID up_long_frames[3] = {SPR_BLOCK_UP, SPR_BLOCK_UP_LONG_30, SPR_BLOCK_UP_LONG_60};
static AnimationSprite anim_up_long = {
    .duration = 2.0f,
    .frameCount = 3,
    .frames = up_long_frames,
    .loop = true,
};

void debug_animation_draw()
{
    if (!debug_animation_initted)
    {
        debug_animation_initted = true;
        anim_up_long.start();
    }

    // draw floors
    for (int y = -2; y < 3; y++)
    {
        for (int x = -2; x < 3; x++)
        {
            int sx;
            int sy;
            IsoToWorld(x, y, 64, 32, &sx, &sy);
            Game::DrawSprite(startX + sx, startY + sy, SPR_FLOOR);
        }
    }

    // draw animation
    Game::DrawSprite(startX, startY, anim_up_long.tick());
}

bool debug_animation2_initted = false;
Animation anim;
int startAnimX = 150;
AnimationProperty<float> animLinear;
AnimationProperty<float> animEaseIn;
AnimationProperty<float> animEaseOut;
AnimationProperty<float> animX;
AnimationProperty<vec2> animV;
AnimationProperty<SpriteID> animSprite;

void debug_animation2_init()
{
    debug_animation2_initted = true;
    anim.duration = 3.0f;
    anim.mode = AnimationPlayMode::PINGPONG;
    anim.start();

    animLinear.addKeyframe(0.0f, startAnimX);
    animLinear.addKeyframe(1.0f, Game::ScreenWidth() - 10.0f);
    animLinear.interpolationType = InterpolationType::LINEAR;

    animEaseIn.addKeyframe(0.0f, startAnimX);
    animEaseIn.addKeyframe(1.0f, Game::ScreenWidth() - 10.0f);
    animEaseIn.interpolationType = InterpolationType::EASE_IN;

    animEaseOut.addKeyframe(0.0f, startAnimX);
    animEaseOut.addKeyframe(1.0f, Game::ScreenWidth() - 10.0f);
    animEaseOut.interpolationType = InterpolationType::EASE_OUT;

    animX.addKeyframesEvenly({10.0f, Game::ScreenWidth() - 10.0f, (Game::ScreenWidth() / 2) - 10.0f});
    animX.interpolationType = InterpolationType::EASE_OUT;

    animV.addKeyframesEvenly({
        {10, 10},
        {Game::ScreenWidth() - 10, 10},
        {Game::ScreenWidth() - 10, Game::ScreenHeight() - 10},
        {10, Game::ScreenHeight() - 10},
    });
    animV.addKeyframe(1.0f, {10, 10});
    animV.interpolationType = InterpolationType::EASE_IN;

    animSprite.addKeyframesEvenly({
        SpriteID::SPR_BLOCK_UP,
        SpriteID::SPR_BLOCK_UP_LONG_30,
        SpriteID::SPR_BLOCK_UP_LONG_60,
    });

    startX = Game::ScreenWidth() / 2;
    startY = Game::ScreenHeight() / 2;
}

void debug_animation2_update(float dt)
{
    if (!debug_animation2_initted)
    {
        debug_animation2_init();
    }

    if (Input::JustPressed(SDL_SCANCODE_F5))
    {
        debug_animation2_initted = false;
        return;
    }

    anim.update(dt);
}

void debug_animation2_draw()
{
    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawLine(Game::GetRenderer(), startAnimX, 590, startAnimX, 650);
    SDL_RenderDrawLine(Game::GetRenderer(), Game::ScreenWidth() - 10, 590, Game::ScreenWidth() - 10, 650);
    Game::Text(10, 600, "LINEAR", {.align = TextAlign::LEFT, .valign = VerticalAlign::MIDDLE});
    Game::Text(animLinear.evaluate(anim.getProgress()), 600, "O",
               {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
    Game::Text(10, 620, "EASE IN", {.align = TextAlign::LEFT, .valign = VerticalAlign::MIDDLE});
    Game::Text(animEaseIn.evaluate(anim.getProgress()), 620, "O",
               {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
    Game::Text(10, 640, "EASE OUT", {.align = TextAlign::LEFT, .valign = VerticalAlign::MIDDLE});
    Game::Text(animEaseOut.evaluate(anim.getProgress()), 640, "O",
               {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});

    float x = animX.evaluate(anim.getProgress());
    Game::DrawSprite(x, 300, SpriteID::SPR_BLOCK_UP);

    // draw floors
    for (int y = -2; y < 3; y++)
    {
        for (int x = -2; x < 3; x++)
        {
            int sx;
            int sy;
            IsoToWorld(x, y, 64, 32, &sx, &sy);
            Game::DrawSprite(startX + sx, startY + sy, SPR_FLOOR);
        }
    }

    SpriteID spr = animSprite.evaluate(anim.getProgress());
    Game::DrawSprite(startX, startY, spr);

    auto pos = animV.evaluate(anim.getProgress());
    Game::Text(pos.x, pos.y, "O", {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});

    Game::Text(Game::ScreenWidth() / 2 - 100, 200, std::format("Animation progress: {:.2f}", anim.getProgress()),
               {.align = TextAlign::LEFT, .valign = VerticalAlign::BOTTOM});
}