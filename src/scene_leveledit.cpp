#include <math.h>
#include <string>

#include "scene_leveledit.hpp"

LevelEditScene::LevelEditScene(Game *game)
    : Scene(game),
      level(game), block(game)
{
    SDL_Log("Loading Level scene\n");

    this->reset();
}

LevelEditScene::~LevelEditScene()
{
}

void LevelEditScene::reset()
{
    auto lvl = game->getOrCreateState("next_level", "1");
    SDL_Log("Loading level %s\n", lvl.c_str());
    auto lvlIdx = std::stoi(lvl) - 1;
    level.load(game->getLevelData(lvlIdx));

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

void toGrid(int worldX, int worldY, int cellSize, int cols, int rows, int *x, int *y)
{
    *x = SDL_floor(worldX / cellSize);
    if (worldX < 0)
        *x -= 1;
    *y = SDL_floor(worldY / cellSize);
    if (worldY < 0)
        *y -= 1;
}

void LevelEditScene::input()
{
    if (game->input.just_pressed(SDL_SCANCODE_E))
    {
        // exit level edit
        game->loadScene(Scenes::ISOLEVEL);
        return;
    }

    game->input.mouse_position(&mousePos.x, &mousePos.y);
    toGrid(mousePos.x - offsetX, mousePos.y - offsetY, cellSize, level.cols, level.rows, &mouseGridPos.x, &mouseGridPos.y);
}

void LevelEditScene::update(float dt)
{
    if (level.isValidPos(mouseGridPos))
    {
        if (game->input.mouse_just_pressed(SDL_BUTTON_LEFT))
        {
            level.toggleFloor(mouseGridPos);
        }

        if (game->input.mouse_just_pressed(SDL_BUTTON_RIGHT))
        {
            level.toggleStartFinish(mouseGridPos);
        }
    }

    // normal op
    moveDir = vec2(0, 0);
    if (game->input.just_pressed(SDL_SCANCODE_UP))
        moveDir = vec2(0, -1);
    if (game->input.just_pressed(SDL_SCANCODE_DOWN))
        moveDir = vec2(0, 1);
    if (game->input.just_pressed(SDL_SCANCODE_LEFT))
        moveDir = vec2(-1, 0);
    if (game->input.just_pressed(SDL_SCANCODE_RIGHT))
        moveDir = vec2(1, 0);

    block.move(moveDir);
    // auto positions = block.getPositions();
    // if (!level.hasFloorAt(positions.first) || !level.hasFloorAt(positions.second))
    //     block.undoMove();
}


void LevelEditScene::draw()
{
    // level outline
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

            switch (level.grid[j][i])
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

    // editor
    if (level.isValidPos(mouseGridPos))
    {
        SDL_SetRenderDrawColor(game->getRenderer(), 255, 100, 0, 255);
        SDL_Rect tileSelect = {
            .x = offsetX + mouseGridPos.x * cellSize,
            .y = offsetY + mouseGridPos.y * cellSize,
            .w = cellSize,
            .h = cellSize};
        SDL_RenderDrawRect(game->getRenderer(), &tileSelect);
    }
}
