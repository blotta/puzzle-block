#include "scene_main_menu.hpp"

static LevelData lvl = {.cols = 7,
                        .rows = 7,
                        .data = {"0001000"
                                 "0001000"
                                 "0001000"
                                 "1112111"
                                 "0001000"
                                 "0001000"
                                 "0001000"}};

void MainMenuScene::init()
{
    Log::info("Loading Menu scene\n");
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
}

void MainMenuScene::dispose()
{
    Log::info("Unloading Menu scene\n");
}

void MainMenuScene::update(float dt)
{
    block.update(dt);

	if (block.currSim.x == 3 && block.currSim.y == 0)
		Game::LoadScene(Scenes::ISOLEVEL);

    camera.update(dt);
}

void MainMenuScene::draw()
{
    level.draw(camera.offset.x - camera.pos.x, camera.offset.y - camera.pos.y, cellSize);
    block.draw(camera.offset.x - camera.pos.x, camera.offset.y - camera.pos.y, cellSize);
}
