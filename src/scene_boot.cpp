#include <SDL2/SDL.h>

#include "asset_manager.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include "scene_boot.hpp"
#include "util.hpp"

#include "animation.hpp"
#include "block.hpp"
#include "particle.hpp"
#include <format>
#include <span>

static BootDebugType bootDebugType = BootDebugType::DEBUG_TYPE_INPUT_TEST;

void debug_input_test_update(float dt);
void debug_input_test_draw();

void debug_dynamic_text_draw();

void debug_sprite_positioning_update(float dt);
void debug_sprite_positioning_draw();

void debug_animation_draw();

void debug_animation2_update(float dt);
void debug_animation2_draw();

void debug_block_update(float dt);
void debug_block_draw();

void debug_particle_update(float dt);
void debug_particle_draw();

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
    case BootDebugType::DEBUG_TYPE_BLOCK:
        return "Block";
    case BootDebugType::DEBUG_TYPE_PARTICLE:
        return "Particle";
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
    case BootDebugType::DEBUG_TYPE_BLOCK:
        this->debug_update_func = debug_block_update;
        this->debug_draw_func = debug_block_draw;
        break;
    case BootDebugType::DEBUG_TYPE_PARTICLE:
        this->debug_update_func = debug_particle_update;
        this->debug_draw_func = debug_particle_draw;
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
static int sprite_positioning_mousex;
static int sprite_positioning_mousey;

void debug_sprite_positioning_update(float dt)
{
    if (!debug_sprite_positioning_initted)
    {
        debug_sprite_positioning_initted = true;
        startX = Game::ScreenWidth() / 2;
        startY = Game::ScreenHeight() / 2;
        spr = Game::GetSprite(SPR_BLOCK_UP);
    }

    Input::MousePosition(&sprite_positioning_mousex, &sprite_positioning_mousey);
    sprite_positioning_mousey -= 20;

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
                           "spritesheet: {} {}\n"
                           "mouse: {}, {}",
                           startX, startY, spr.originX, spr.originY, spr.tx / 128, (spr.ty - 64) / 128,
                           sprite_positioning_mousex - startX, sprite_positioning_mousey - startY));

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

    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawPoint(Game::GetRenderer(), sprite_positioning_mousex, sprite_positioning_mousey);
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
AnimationProperty<float> animEaseOutBack;
AnimationProperty<float> animSnap;
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
    anim.onComplete = []() { Log::debug("Animation completed\n"); };

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

    animEaseOutBack.addKeyframe(0.0f, 0.0f);
    animEaseOutBack.addKeyframe(1.0f, 1.0f);
    animEaseOutBack.interpolationType = InterpolationType::EASE_OUT_BACK;

    animSnap.addKeyframe(0.0f, 0.0f);
    animSnap.addKeyframe(1.0f, 1.0f);
    animSnap.interpolationType = InterpolationType::SNAP;

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
            vec2{10, 10},
            vec2{Game::ScreenWidth() - 10, 10},
            vec2{Game::ScreenWidth() - 10, Game::ScreenHeight() - 10},
            vec2{10, Game::ScreenHeight() - 10},
            vec2{10, 10},
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
    int startAnimX = 300;
    int startAnimY = 600;
    int endAnimX = Game::ScreenWidth() - 100;
    int length = endAnimX - startAnimX;
    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    Game::Text(10, startAnimY + 20 * 0.f, "LINEAR", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, startAnimY + 20 * 1.f, "EASE IN", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, startAnimY + 20 * 2.f, "EASE OUT", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, startAnimY + 20 * 3.f, "EASE OUT BOUNCE", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, startAnimY + 20 * 4.f, "EASE OUT BACK", {.valign = VerticalAlign::MIDDLE});
    Game::Text(10, startAnimY + 20 * 5.f, "SNAP", {.valign = VerticalAlign::MIDDLE});
    SDL_FRect rects[] = {
        {startAnimX + (length - 10.f) * animLinear.evaluate(anim.getProgress()), startAnimY + 20 * 0.f, 10.f, 10.f},
        {startAnimX + (length - 10.f) * animEaseIn.evaluate(anim.getProgress()), startAnimY + 20 * 1.f, 10.f, 10.f},
        {startAnimX + (length - 10.f) * animEaseOut.evaluate(anim.getProgress()), startAnimY + 20 * 2.f, 10.f, 10.f},
        {startAnimX + (length - 10.f) * animEaseOutBounce.evaluate(anim.getProgress()), startAnimY + 20 * 3.f, 10.f,
         10.f},
        {startAnimX + (length - 10.f) * animEaseOutBack.evaluate(anim.getProgress()), startAnimY + 20 * 4.f, 10.f,
         10.f},
        {startAnimX + (length - 10.f) * animSnap.evaluate(anim.getProgress()), startAnimY + 20 * 5.f, 10.f, 10.f},
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
    Game::Text(progressBorderRect.x, progressBorderRect.y - 20,
               std::format("Animation progress: {:.2f}", anim.getProgress()), {.valign = VerticalAlign::BOTTOM});

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

///////////
// Block //
///////////

static bool debug_block_initted = false;
static BlockVisual blockVisual;

void debug_block_update(float dt)
{
    if (!debug_block_initted)
    {
        blockVisual.init({0, 0}, BlockState::UP);
        startX = Game::ScreenWidth() / 2;
        startY = Game::ScreenHeight() / 2;
        debug_block_initted = true;
    }

    if (Input::JustPressed(SDL_SCANCODE_F5))
    {
        debug_block_initted = false;
        return;
    }

    blockVisual.update(dt);
}

void debug_block_draw()
{
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

    // draw block
    blockVisual.draw(startX, startY, 64);
}

/////////////////////
// PARTICLE SYSTEM //
/////////////////////

static bool debug_particle_initted = false;
static ParticleSystem particle_particleSystem;
static BlockVisual particle_blockVisual;

void debug_particle_update(float dt)
{
    if (!debug_particle_initted)
    {
        startX = Game::ScreenWidth() / 2;
        startY = Game::ScreenHeight() / 2;
        particle_blockVisual.init({0, 0}, BlockState::UP);
        particle_particleSystem.startColor = {220, 200, 200, 255};
        particle_particleSystem.endColor = {220, 200, 200, 0};
        particle_particleSystem.vx = 5.f;
        particle_particleSystem.vy = 2.f;
        particle_particleSystem.gravity = 2.f;
        particle_particleSystem.minLife = 0.3f;
        particle_particleSystem.maxLife = 0.5f;
        particle_blockVisual.startFall([]() {
            int x, y;
            IsoToWorld(particle_blockVisual.currSim.x, particle_blockVisual.currSim.y, 64, 32, &x, &y);
            x += startX;
            y += startY;

            float x1 = x + 0;
            float y1 = y + 16;
            float x2 = x + 32;
            float y2 = y + 32;
            float x3 = x + 64;
            float y3 = y + 16;
            particle_particleSystem.emitLine(x1, y1, x2, y2, 50);
            particle_particleSystem.emitLine(x2, y2, x3, y3, 50);
        });
        debug_particle_initted = true;
    }

    if (Input::JustPressed(SDL_SCANCODE_F5))
    {
        debug_particle_initted = false;
        return;
    }

    if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
    {
        int mx, my;
        Input::MousePosition(&mx, &my);
        // particle_particleSystem.emit(mx, my, 100);
        particle_particleSystem.emitLine(mx - 50, my, mx + 50, my, 100);
    }

    particle_blockVisual.update(dt);

    particle_particleSystem.update(dt);
}

void debug_particle_draw()
{
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

    // draw block
    particle_blockVisual.draw(startX, startY, 64);

    particle_particleSystem.draw();
}