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

void MainMenuScene::init()
{
    Log::info("Loading Menu scene\n");
    Game::PlayMusic("assets/sfx/music_ambient_01.ogg");
    Game::SetFontSize(20);

    level.load(lvl);

    // block setup
    auto startPos = level.getStartPos();
    block.init(startPos, BlockState::UP);
    block.level = &level;

    camera.offset = vec2(Game::ScreenWidth() / 2, Game::ScreenHeight() / 2);
    int tx, ty;
    IsoToWorld(startPos.x, startPos.y, cellSize, cellSize / 2, &tx, &ty);
    camera.target.x = tx;
    camera.target.y = ty;

    choseOption = false;

    block.startFall();
}

void MainMenuScene::dispose()
{
    Log::info("Unloading Menu scene\n");
}

void MainMenuScene::onPopReturn()
{
    auto startPos = level.getStartPos();
    block.init(startPos, BlockState::UP);
    block.startFall();
    choseOption = false;
}

void MainMenuScene::update(float dt)
{
    block.update(dt);

    if (!choseOption)
    {
        // Start
        if (block.currSim.x == 3 && block.currSim.y == 0)
        {
            choseOption = true;
            block.startFly([](){
                Game::LoadScene(Scenes::ISOLEVEL);
            });
        }

        // Exit
        if (block.currSim.x == 3 && block.currSim.y == 6)
        {
            choseOption = true;
            block.startFly([](){
                Game::Exit();
            });
        }

        // Options
        if (block.currSim.x == 0 && block.currSim.y == 3)
        {
            choseOption = true;
            block.startFly([](){
                Game::PushScene(Scenes::OPTIONS);
            });
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
