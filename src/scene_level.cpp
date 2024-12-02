#include <math.h>
#include <string>

#include "scene_level.hpp"

LevelScene::LevelScene(Game *game)
    :
    Scene(game)
    ,
    level(game)
    , block(game)
{
    SDL_Log("Loading Level scene\n");

    this->reset();
}

LevelScene::~LevelScene()
{
}

void LevelScene::reset()
{
    auto lvl = game->getOrCreateState("next_level", "1");

    auto lvlIdx = std::stoi(lvl) - 1;

    level.load(LEVELS[lvlIdx]);

    // view sizes
    int hor = SDL_floorf(game->ScreenWidth * 0.8) / level.cols;
    int vert = SDL_floorf(game->ScreenHeight * 0.8) / level.rows;
    cellSize = hor > vert ? vert : hor;
    offsetX = game->ScreenWidth / 2 - level.cols * cellSize / 2;
    offsetY = game->ScreenHeight / 2 - level.rows * cellSize / 2;

    auto startPos = level.getStartPos();
    block.x = startPos.x;
    block.y = startPos.y;
    block.state = BlockState::UP;
}

void LevelScene::input()
{
    if (game->input.just_pressed(SDL_SCANCODE_1))
    {
        game->loadScene(Scenes::BOOT);
    }

    moveDir = vec2(0, 0);
    if (game->input.just_pressed(SDL_SCANCODE_UP))
        moveDir = vec2(0, -1);
    if (game->input.just_pressed(SDL_SCANCODE_DOWN))
        moveDir = vec2(0, 1);
    if (game->input.just_pressed(SDL_SCANCODE_LEFT))
        moveDir = vec2(-1, 0);
    if (game->input.just_pressed(SDL_SCANCODE_RIGHT))
        moveDir = vec2(1, 0);
}


void LevelScene::update(float dt)
{
    block.move(moveDir);
    auto positions = block.getPositions();
    if (!level.hasFloorAt(positions.first) || !level.hasFloorAt(positions.second))
        block.undoMove();
}

void LevelScene::draw()
{
    // draw level
    level.draw(game->getRenderer(), offsetX, offsetY, cellSize);

    // draw block
    block.draw(game->getRenderer(), offsetX, offsetY, cellSize);
}
