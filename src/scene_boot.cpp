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

BootScene::BootScene()
{
}

BootScene::~BootScene()
{
}

BootScene::BootScene(BootScene&& other) noexcept
    : Scene(std::move(other)), timer(other.timer), loadFirstScene(other.loadFirstScene),
      debugModeActive(other.debugModeActive), debug_update_func(std::move(other.debug_update_func)),
      debug_draw_func(std::move(other.debug_draw_func))
{
    other.debug_draw_func = nullptr;
    other.debug_update_func = nullptr;
}

void BootScene::init()
{
    timer.setDuration(3);
    timer.reset();

    Game::SetFontSize(20);

    setDebugType(bootDebugType);
    Log::debug("Loading boot scene\n");
}

void BootScene::dispose()
{
    Log::debug("Unloading boot scene\n");
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
    .duration = 3.0f,
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
AnimationProperty<float> animLinear;
AnimationProperty<float> animEaseIn;
AnimationProperty<float> animEaseOut;
AnimationProperty<float> animEaseOutBounce;
AnimationProperty<vec2> animG;
AnimationProperty<vec2> animV;
AnimationProperty<SpriteID> animSprite;

void debug_animation2_init()
{
    startX = Game::ScreenWidth() / 2;
    startY = Game::ScreenHeight() / 2;

    debug_animation2_initted = true;
    anim.duration = 3.0f;
    anim.mode = AnimationPlayMode::LOOP;
    anim.start();
    anim.onComplete = []() {
        Log::debug("Animation completed\n");
    };

    animLinear.addKeyframe(0.0f, 0.0f);
    animLinear.addKeyframe(1.0f, 1.0f);
    animLinear.interpolationType = InterpolationType::LINEAR;

    animEaseIn.addKeyframe(0.0f, 0.0f);
    animEaseIn.addKeyframe(1.0f, 1.0f);
    animEaseIn.interpolationType = InterpolationType::EASE_IN;

    animEaseOut.addKeyframe(0.0f, 0.0f);
    animEaseOut.addKeyframe(1.0f, 1.0f);
    animEaseOut.interpolationType = InterpolationType::EASE_OUT;

    animEaseOutBounce.addKeyframe(0.0f, 0.0f);
    animEaseOutBounce.addKeyframe(1.0f, 1.0f);
    animEaseOutBounce.interpolationType = InterpolationType::EASE_OUT_BOUNCE;

    vec2 gridPoints[] = {vec2{-2, -2}, vec2{2, -2}, vec2{2, 2}, vec2{-2, 2}, vec2{-2, -2}};
    int gridPointsCount = sizeof(gridPoints) / sizeof(gridPoints[0]);
    for (int i = 0; i < gridPointsCount; i++)
    {
        int sx, sy;
        IsoToWorld(gridPoints[i].x, gridPoints[i].y, 64, 32, &sx, &sy);
        gridPoints[i].x = startX + sx;
        gridPoints[i].y = startY + sy;
    }
    animG.addKeyframesEvenly(gridPoints, true);
    animG.interpolationType = InterpolationType::EASE_OUT;

    animV.addKeyframesEvenly(
        {
            {10, 10},
            {Game::ScreenWidth() - 10, 10},
            {Game::ScreenWidth() - 10, Game::ScreenHeight() - 10},
            {10, Game::ScreenHeight() - 10},
            {10, 10},
        },
        true);
    animV.interpolationType = InterpolationType::EASE_IN;

    animSprite.addKeyframesEvenly({
        SpriteID::SPR_BLOCK_UP,
        SpriteID::SPR_BLOCK_UP_LONG_30,
        SpriteID::SPR_BLOCK_UP_LONG_60,
    });
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

    if (Input::JustPressed(SDL_SCANCODE_1))
    {
        anim.mode = AnimationPlayMode::ONCE;
        anim.start();
    }
    if (Input::JustPressed(SDL_SCANCODE_2))
    {
        anim.mode = AnimationPlayMode::LOOP;
        anim.start();
    }
    if (Input::JustPressed(SDL_SCANCODE_3))
    {
        anim.mode = AnimationPlayMode::PINGPONG;
        anim.start();
    }

    anim.update(dt);
}

void debug_animation2_draw()
{
    // easings comparison
    int startAnimX = 200;
    int startAnimY = 600;
    int endAnimX = Game::ScreenWidth() - 10;
    int length = endAnimX - startAnimX;
    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    Game::Text(10, 600, "LINEAR", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, 620, "EASE IN", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, 640, "EASE OUT", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, 660, "EASE OUT BOUNCE", {.valign = VerticalAlign::MIDDLE});
    SDL_FRect rects[] = {
        {startAnimX + (length - 10.f) * animLinear.evaluate(anim.getProgress()), startAnimY + 20 * 0.f, 10.f, 10.f},
        {startAnimX + (length - 10.f) * animEaseIn.evaluate(anim.getProgress()), startAnimY + 20 * 1.f, 10.f, 10.f},
        {startAnimX + (length - 10.f) * animEaseOut.evaluate(anim.getProgress()), startAnimY + 20 * 2.f, 10.f, 10.f},
        {startAnimX + (length - 10.f) * animEaseOutBounce.evaluate(anim.getProgress()), startAnimY + 20 * 3.f, 10.f,
         10.f},
    };
    int lengthRects = sizeof(rects) / sizeof(rects[0]);
    SDL_RenderFillRectsF(Game::GetRenderer(), rects, lengthRects);

    SDL_RenderDrawLine(Game::GetRenderer(), startAnimX, startAnimY - 10, startAnimX,
                       startAnimY + lengthRects * 20 + 10);
    SDL_RenderDrawLine(Game::GetRenderer(), endAnimX, startAnimY - 10, endAnimX, startAnimY + lengthRects * 20 + 10);

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

    // sprite animation
    SpriteID spr = animSprite.evaluate(anim.getProgress());

    // grid animation
    auto gridWorldPos = animG.evaluate(anim.getProgress());

    // draw ordering
    if (gridWorldPos.y < startY)
        Game::DrawSprite(gridWorldPos.x, gridWorldPos.y, SpriteID::SPR_BLOCK_UP);

    Game::DrawSprite(startX, startY, spr);

    int bounceX, bounceY;
    IsoToWorld(-1, 1, 64, 32, &bounceX, &bounceY);
    float bounceProg = 1 - animEaseOutBounce.evaluate(anim.getProgress());
    float bounceHeight = 500 * bounceProg;
    Game::DrawSprite(startX + bounceX, startY + bounceY - bounceHeight, SpriteID::SPR_BLOCK_UP);

    if (gridWorldPos.y >= startY)
        Game::DrawSprite(gridWorldPos.x, gridWorldPos.y, SpriteID::SPR_BLOCK_UP);

    // border square animation
    auto pos = animV.evaluate(anim.getProgress());
    SDL_Rect rect = {pos.x - 10, pos.y - 10, 20, 20};
    SDL_RenderFillRect(Game::GetRenderer(), &rect);

    // progress bar
    SDL_Rect progressBorderRect = {.x = Game::ScreenWidth() / 2 - 100, .y = 150, .w = 200, .h = 20};
    SDL_Rect progressFillRect = {.x = progressBorderRect.x,
                                 .y = progressBorderRect.y,
                                 .w = (int)(progressBorderRect.w * anim.getProgress()),
                                 .h = progressBorderRect.h};

    SDL_RenderDrawRect(Game::GetRenderer(), &progressBorderRect);
    SDL_RenderFillRect(Game::GetRenderer(), &progressFillRect);
    Game::Text(progressBorderRect.x, progressBorderRect.y - 20, std::format("Animation progress: {:.2f}", anim.getProgress()),
               {.valign = VerticalAlign::BOTTOM});

    // top left controls. yellow if selected
    Game::Text(
        10, 10, "1: ONCE",
        {.color = anim.mode == AnimationPlayMode::ONCE ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255}});
    Game::Text(
        10, 30, "2: LOOP",
        {.color = anim.mode == AnimationPlayMode::LOOP ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255}});
    Game::Text(10, 50, "3: PINGPONG",
               {.color = anim.mode == AnimationPlayMode::PINGPONG ? SDL_Color{255, 255, 0, 255}
                                                                  : SDL_Color{255, 255, 255, 255}});
    Game::Text(10, 70, "Press F5 to reset");
}