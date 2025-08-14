#include "scene_main_menu.hpp"

static LevelData lvl = {.cols = 7,
                        .rows = 7,
                        .data = {"0003000"
                                 "0001000"
                                 "0001000"
                                 "3112113"
                                 "0001000"
                                 "0001000"
                                 "0003000"}};

static bool choseOption = false;
static Scenes sceneChosen = Scenes::NONE;

void MainMenuScene::init()
{
    Log::info("Loading Menu scene\n");
    Game::PlayMusic("assets/sfx/music_ambient_01.ogg");

    level.load(lvl);

    // block setup
    auto startPos = level.getStartPos();
    block.init(startPos, BlockState::UP);
    block.level = &level;

    camera.offset = vec2(Game::ScreenWidth() / 2, Game::ScreenHeight() / 2);
    int tx, ty;
    IsoToWorld(block.currSim.x, block.currSim.y, cellSize, cellSize / 2, &tx, &ty);
    camera.target.x = tx;
    camera.target.y = ty;

    block.startFall();
}

void MainMenuScene::dispose()
{
    Log::info("Unloading Menu scene\n");
}

void MainMenuScene::update(float dt)
{
    block.update(dt);

    if (!choseOption)
    {
        if (block.currSim.x == 3 && block.currSim.y == 0)
        {
            sceneChosen = Scenes::ISOLEVEL;
            choseOption = true;
        }

        if (sceneChosen != Scenes::NONE)
        {
            block.startFly([]() { Game::LoadScene(Scenes::ISOLEVEL); });
            sceneChosen = Scenes::NONE;
        }
    }

    camera.update(dt);
}

void MainMenuScene::draw()
{
    level.draw(camera.offset.x - camera.pos.x, camera.offset.y - camera.pos.y, cellSize);
    block.draw(camera.offset.x - camera.pos.x, camera.offset.y - camera.pos.y, cellSize);

    int txtX, txtY;

    IsoToWorld(3, 0, cellSize, cellSize / 2, &txtX, &txtY);
    Game::Text(txtX + cellSize * 1.5 + camera.offset.x - camera.pos.x, txtY - cellSize / 2 + camera.offset.y - camera.pos.y,
               "START");

    IsoToWorld(3, 6, cellSize, cellSize / 2, &txtX, &txtY);
    Game::Text(txtX - cellSize/2 + camera.offset.x - camera.pos.x, txtY + cellSize / 2 + camera.offset.y - camera.pos.y,
               "EXIT", {.align = TextAlign::RIGHT});

    IsoToWorld(0, 3, cellSize, cellSize / 2, &txtX, &txtY);
    Game::Text(txtX - cellSize/2 + camera.offset.x - camera.pos.x, txtY - cellSize / 2 + camera.offset.y - camera.pos.y,
               "OPTIONS", {.align = TextAlign::RIGHT});

    IsoToWorld(6, 3, cellSize, cellSize / 2, &txtX, &txtY);
    Game::Text(txtX + cellSize * 1.5 + camera.offset.x - camera.pos.x, txtY + cellSize / 2 + camera.offset.y - camera.pos.y,
               "LEVEL SELECT");
}
