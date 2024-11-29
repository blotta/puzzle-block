#include <math.h>
#include <string>

#include "scene_level.hpp"

LevelScene::LevelScene(Game *game)
    : Scene(game)
{
    SDL_Log("Loading Level scene\n");

    this->reset();
}

LevelScene::~LevelScene()
{
}

void LevelScene::reset()
{
    auto lvl = game->getOrCreateState("next_level", "2");

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
    if (moveDir.x != 0 || moveDir.y != 0)
    {
        block.move(moveDir);
    }
}

void LevelScene::draw()
{
    SDL_Rect rr = {offsetX - 1, offsetY - 1, cellSize * level.cols + 2, cellSize * level.rows + 2};
    SDL_SetRenderDrawColor(game->getRenderer(), 40, 200, 80, 255);
    SDL_RenderDrawRect(game->getRenderer(), &rr);

    // draw level
    for (int j = 0; j < level.rows; j++)
    {
        for (int i = 0; i < level.cols; i++)
        {
            SDL_Rect r = {
                offsetX + cellSize * i,
                offsetY + cellSize * j,
                cellSize,
                cellSize};
            
            switch (level.mGrid[j][i])
            {
            case CellType::FLOOR: // floor
                SDL_SetRenderDrawColor(game->getRenderer(), 100, 100, 100, 255);
                break;
            case CellType::START: // start
                SDL_SetRenderDrawColor(game->getRenderer(), 100, 100, 200, 255);
                break;
            case CellType::FINISH: // finish
                SDL_SetRenderDrawColor(game->getRenderer(), 100, 200, 100, 255);
                break;
            case CellType::EMPTY:
            default: // empty
                SDL_SetRenderDrawColor(game->getRenderer(), 10, 10, 10, 255);
                break;
            }
            SDL_RenderFillRect(game->getRenderer(), &r);
            SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
            SDL_RenderDrawRect(game->getRenderer(), &r);
        }
    }

    // draw block
    SDL_SetRenderDrawColor(game->getRenderer(), 200, 100, 100, 255);
    SDL_Rect blockRect = {
        offsetX + block.x * cellSize,
        offsetY + block.y * cellSize,
        cellSize,
        cellSize
    };
    if (block.state == BlockState::WIDE)
        blockRect.w *= 2;
    if (block.state == BlockState::LONG)
        blockRect.h *= 2;
    SDL_RenderFillRect(game->getRenderer(), &blockRect);
}
