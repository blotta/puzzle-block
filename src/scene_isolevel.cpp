#include "scene_isolevel.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include <format>

static const int cellSize = 64;

void IsoLevelScene::init()
{
    Log::info("Loading IsoLevel scene\n");
    this->reset();

    Game::PlayMusic("assets/sfx/music_ambient_01.ogg");
    Game::SetFontSize(32);

    auto textHalfWidth = Game::TextWidth("LEVEL CLEARED") / 2 + 10;
    animLevelClearedProp.addKeyframe(0.15f, -textHalfWidth); // Level Cleared text position
    animLevelClearedProp.addKeyframe(0.20f, Game::ScreenWidth() / 2 - 50);
    animLevelClearedProp.addKeyframe(0.60f, Game::ScreenWidth() / 2);
    animLevelClearedProp.addKeyframe(0.65f, Game::ScreenWidth() + textHalfWidth);
    animLevelCleared.addEvent(0.2f, []() { Game::PlaySound("assets/sfx/arrive.ogg"); });
    animLevelCleared.onComplete = []() { Game::LoadScene(Scenes::ISOLEVEL); };
    animLevelCleared.duration = 3.f;
    animLevelClearedProp.interpolationType = InterpolationType::EASE_OUT;
    animLevelCleared.stop();
}

void IsoLevelScene::dispose()
{
    Log::info("Unloading IsoLevel scene\n");
}

void IsoLevelScene::reset()
{
    auto lvl = Game::GetOrCreateState("curr_level", "0");
    int lvlIdx = std::stoi(lvl);
    Log::info("Loading level %d\n", lvlIdx + 1);
    level.init(Game::GetLevelData(lvlIdx));

    mTitleText = std::format("Level {}", lvlIdx + 1);

    // block setup
    auto startPos = level.mModel.getStartPos();
    block.init(startPos, BlockState::UP);
    block.level = &level.mModel;

    mLevelCleared = false;

    block.vState = BlockVisualState::INVISIBLE;
    level.startRise([this]() { block.startFall(); });
}

void IsoLevelScene::update(float dt)
{
    block.update(dt);
    level.update(dt);

    if (mLevelCleared)
    {
        animLevelCleared.update(dt);
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_ESCAPE))
    {
        Game::PushScene(Scenes::PAUSE);
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_E))
    {
        // edit this level
        Game::LoadScene(Scenes::LEVEL_EDIT);
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_L))
    {
        // next level
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), 1);
        Game::SetState("curr_level", std::to_string(idx));
        reset();
    }
    else if (Input::JustPressed(SDL_SCANCODE_H))
    {
        // previous level
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), -1);
        Game::SetState("curr_level", std::to_string(idx));
        reset();
    }


    if (block.moved)
    {
        auto positions = block.currSim.getPositions();
        level.mModel.checkAndTriggerSwitches(positions.first, positions.second);

        if (level.mModel.isCleared(positions.first, positions.second))
        {
            this->mLevelCleared = true;
            int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), 1);
            Game::SetState("curr_level", std::to_string(idx));
            Log::debug("Level cleared!");
            block.startFly([this]() {
                this->animLevelCleared.start();
                Log::debug("starting level cleared animation");
            });
        }
    }

    if (Input::MousePressed(SDL_BUTTON_LEFT))
    {
        vec2 mouseScreenPos;
        Input::MousePosition(&mouseScreenPos.x, &mouseScreenPos.y);
        auto mouseWorldPos = Game::ScreenToWorld(mouseScreenPos);
        mouseWorldPos = mouseWorldPos * 0.5f;
        Game::CameraSetTarget(mouseWorldPos);
    }
    else
    {
        int tx, ty;
        IsoToWorld(block.nextSim.x, block.nextSim.y, cellSize, cellSize / 2, &tx, &ty);
        Game::CameraSetTarget(vec2f{(float)(tx + cellSize/2), (float)ty});
    }
}

void IsoLevelScene::draw()
{

    Game::BeginCamera();

    level.draw();
    block.draw();

    Game::EndCamera();

    Game::Text(Game::ScreenWidth() / 2, 10, mTitleText, {.align = TextAlign::CENTER});

    if (animLevelCleared.playing)
    {
        float x = animLevelClearedProp.evaluate(animLevelCleared.getProgress());
        Game::Text(x, Game::ScreenHeight() / 4, "LEVEL CLEARED",
                   {.color = {200, 255, 200, 255}, .align = TextAlign::CENTER});
    }
}
