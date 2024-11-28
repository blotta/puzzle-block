#include <math.h>
#include <string>

#include "scene_level.hpp"

LevelScene::LevelScene(Game *game)
    : Scene(game)
{
    SDL_Log("Loading Level scene\n");

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

LevelScene::~LevelScene()
{
}

void LevelScene::input()
{
    if (game->input.just_pressed(SDL_SCANCODE_1))
    {
        game->loadScene(Scenes::BOOT);
    }

    moveDir = MoveDir::NONE;
    if (game->input.just_pressed(SDL_SCANCODE_UP))
        moveDir = MoveDir::MV_UP;
    if (game->input.just_pressed(SDL_SCANCODE_DOWN))
        moveDir = MoveDir::DOWN;
    if (game->input.just_pressed(SDL_SCANCODE_LEFT))
        moveDir = MoveDir::LEFT;
    if (game->input.just_pressed(SDL_SCANCODE_RIGHT))
        moveDir = MoveDir::RIGHT;
}

void LevelScene::update(float dt)
{
    switch (moveDir)
    {
        case MoveDir::MV_UP:
        case MoveDir::DOWN:
        {
            if (block.state == BlockState::UP)
            {
                block.y = moveDir == MoveDir::MV_UP ? block.y - 2 : block.y + 1;
                block.state = BlockState::LONG;
            }
            else if (block.state == BlockState::LONG)
            {
                block.y = moveDir == MoveDir::MV_UP ? block.y - 1 : block.y + 2;
                block.state = BlockState::UP;
            }
            else if (block.state == BlockState::WIDE)
            {
                block.y = moveDir == MoveDir::MV_UP ? block.y - 1 : block.y + 1;
            }
        }
        break;

        case MoveDir::LEFT:
        case MoveDir::RIGHT:
        {
            if (block.state == BlockState::UP)
            {
                block.x = moveDir == MoveDir::LEFT ? block.x - 2 : block.x + 1;
                block.state = BlockState::WIDE;
            }
            else if (block.state == BlockState::LONG)
            {
                block.x = moveDir == MoveDir::LEFT ? block.x - 1 : block.x + 1;
            }
            else if (block.state == BlockState::WIDE)
            {
                block.x = moveDir == MoveDir::LEFT ? block.x - 1 : block.x + 2;
                block.state = BlockState::UP;
            }
        }
        break;

        default:
        break;
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
