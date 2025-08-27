#include "scene_main_menu.hpp"
#include "input_manager.hpp"
#include "component_block.hpp"

static const int cellSize = 64;

static LevelData lvl = {.cols = 7,
                        .rows = 7,
                        .data = {"0003000"
                                 "0001000"
                                 "0001000"
                                 "3112113"
                                 "0001000"
                                 "0001000"
                                 "0003000"}};

static bool showText = false;

void MainMenuScene::preload()
{
    auto level = entities.add("level");
    level->addComponent<CTransform>();
    auto lc = level->addComponent<LevelComponent>(-1, lvl);

    auto block = entities.add("block");
    block->addComponent<CTransform>();
    auto bc = block->addComponent<BlockComponent>();

    bc->level = lc;
    lc->block = bc;

    Game::SetFontSize(20);
}

void MainMenuScene::init()
{
    Log::info("Loading Menu scene\n");
    Game::PlayMusic("assets/sfx/music_ambient_01.ogg");

    showText = false;

    auto lc = entities.get("level")[0]->getComponent<LevelComponent>();
    auto bc = entities.get("block")[0]->getComponent<BlockComponent>();

    lc->onLevelCleared = [bc]() {
        showText = false;
        vec2 optionPos(bc->currSim.x, bc->currSim.y);

        // Start
        if (optionPos == vec2{3, 0})
        {
            bc->startFly([]() { Game::LoadScene(Scenes::LEVEL); });
        }

        // Exit
        if (optionPos == vec2{3, 6})
        {
            bc->startFly([]() { Game::Exit(); });
        }

        // Options
        if (optionPos == vec2{0, 3})
        {
            bc->startFly([]() { Game::PushScene(Scenes::OPTIONS); });
        }

        // Level Select
        if (optionPos == vec2{6, 3})
        {
            bc->startFly([]() { Game::PushScene(Scenes::LEVEL_SELECT); });
        }
    };

    vec2 camPos(cellSize/2, 0);
    Game::CameraSetPosition(camPos);

    bc->vState = BlockVisualState::INVISIBLE;
    lc->startRise([bc]() {
        bc->startFall([](){
            showText = true;
        });
    });
}


void MainMenuScene::dispose()
{
    Log::info("Unloading Menu scene\n");
}

void MainMenuScene::onPopReturn()
{
    showText = false;

    auto lc = entities.get("level")[0]->getComponent<LevelComponent>();
    auto bc = entities.get("block")[0]->getComponent<BlockComponent>();

    auto startPos = lc->mModel.getStartPos();
    bc->currSim.x = startPos.x;
    bc->currSim.y = startPos.y;

    bc->vState = BlockVisualState::INVISIBLE;
    bc->startFall([](){
        showText = true;
    });
}

void MainMenuScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_F11))
    {
        Game::LoadScene(Scenes::LEVEL_EDIT_NEW);
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_F10))
    {
        Game::LoadScene(Scenes::LEVEL);
        return;
    }
}

void MainMenuScene::draw()
{
    int txtX, txtY;

    if (showText)
    {
        IsoToWorld(3, 0, cellSize, cellSize / 2, &txtX, &txtY);
        Game::Text(txtX + cellSize * 1.5, txtY - cellSize / 2, "START");

        IsoToWorld(3, 6, cellSize, cellSize / 2, &txtX, &txtY);
        Game::Text(txtX - cellSize / 2, txtY + cellSize / 2, "EXIT", {.align = TextAlign::RIGHT});

        IsoToWorld(0, 3, cellSize, cellSize / 2, &txtX, &txtY);
        Game::Text(txtX - cellSize / 2, txtY - cellSize / 2, "OPTIONS", {.align = TextAlign::RIGHT});

        IsoToWorld(6, 3, cellSize, cellSize / 2, &txtX, &txtY);
        Game::Text(txtX + cellSize * 1.5, txtY + cellSize / 2, "LEVEL SELECT");
    }
}

void MainMenuScene::drawGUI()
{
    Sprite titleSprite = Game::GetSprite(SPR_TITLE);
    Game::DrawSprite(Game::ScreenWidth() / 2 - titleSprite.tw / 2, 50, SPR_TITLE);
}
