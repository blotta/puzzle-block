
#include "scene_level.hpp"
#include "entity.hpp"
#include "input_manager.hpp"
#include "component_block.hpp"
#include "component_level.hpp"
#include <format>

static const int cellSize = 64;

struct AnimLevelCleared : public Component
{
    Animation anim;
    AnimationProperty<float> animProp;

    void init() override
    {
        auto textHalfWidth = Game::TextWidth("LEVEL CLEARED") / 2 + 10;
        anim.duration = 3.f;
        anim.addEvent(0.2f, []() { Game::PlaySound("assets/sfx/arrive.ogg"); });
        // anim.onComplete = []() { Game::LoadScene(Scenes::ISOLEVEL); };
        anim.stop();

        animProp.interpolationType = InterpolationType::EASE_OUT;
        animProp.addKeyframe(0.15f, -textHalfWidth); // Level Cleared text position
        animProp.addKeyframe(0.20f, Game::ScreenWidth() / 2 - 50);
        animProp.addKeyframe(0.60f, Game::ScreenWidth() / 2);
        animProp.addKeyframe(0.65f, Game::ScreenWidth() + textHalfWidth);
    }

    void update(float dt) override
    {
        anim.update(dt);
    }

    void drawGUI() override
    {
        if (anim.playing)
        {
            float x = animProp.evaluate(anim.getProgress());
            Game::SetFontSize(32);
            Game::Text(x, Game::ScreenHeight() / 4, "LEVEL CLEARED",
                       {.color = {200, 255, 200, 255}, .align = TextAlign::CENTER});
        }
    }
};

void LevelScene::preload()
{
    auto lvlIdx = std::stoi(Game::GetOrCreateState("curr_level", "0"));
    Log::info("Loading level %d\n", lvlIdx + 1);
    title = std::format("Level {}", lvlIdx + 1);

    auto level = entities.add("level");
    level->addComponent<CTransform>();
    this->lc = level->addComponent<LevelComponent>(lvlIdx, Game::GetLevelData(lvlIdx));

    auto block = entities.add("block");
    block->addComponent<CTransform>();
    this->bc = block->addComponent<BlockComponent>();

    bc->level = lc;
    lc->block = bc;

    auto animCleared = entities.add("animCleared");
    animCleared->addComponent<CTransform>();
    auto a = animCleared->addComponent<AnimLevelCleared>();
    a->anim.onComplete = [](){
        Game::LoadScene(Scenes::LEVEL);
    };

    Game::SetFontSize(32);
}

void LevelScene::init()
{
    auto animCleared = entities.get("animCleared")[0]->getComponent<AnimLevelCleared>();

    lc->onLevelCleared = [this, animCleared]() {
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), 1);
        Game::SetState("curr_level", std::to_string(idx));
        this->bc->startFly([animCleared]() {
            animCleared->anim.start();
            Log::debug("starting level cleared animation");
        });
    };

    bc->vState = BlockVisualState::INVISIBLE;
    lc->startRise([this]() { this->bc->startFall(); });
}

void LevelScene::update(float dt)
{
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
        Game::LoadScene(Scenes::LEVEL);
    }
    else if (Input::JustPressed(SDL_SCANCODE_H))
    {
        // previous level
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), -1);
        Game::SetState("curr_level", std::to_string(idx));
        Game::LoadScene(Scenes::LEVEL);
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
        IsoToWorld(this->bc->nextSim.x, this->bc->nextSim.y, cellSize, cellSize / 2, &tx, &ty);
        Game::CameraSetTarget(vec2f{(float)(tx + cellSize/2), (float)ty});
    }
}

void LevelScene::drawGUI()
{
    Game::SetFontSize(32);
    Game::Text(Game::ScreenWidth() / 2, 10, title, {.align = TextAlign::CENTER});
}

void LevelScene::dispose()
{
}
