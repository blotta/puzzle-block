#include <math.h>
#include <string>

#include "scene_level.hpp"
#include "level.hpp"

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
}

LevelScene::~LevelScene()
{
}

void LevelScene::input()
{
}

void LevelScene::update(float dt)
{
}

void LevelScene::draw()
{
    SDL_Rect rr = {offsetX - 1, offsetY - 1, cellSize * level.cols + 2, cellSize * level.rows + 2};
    SDL_SetRenderDrawColor(game->getRenderer(), 40, 200, 80, 255);
    SDL_RenderDrawRect(game->getRenderer(), &rr);

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
            case 1:
                SDL_SetRenderDrawColor(game->getRenderer(), 100, 100, 100, 255);
                break;
            default:
                SDL_SetRenderDrawColor(game->getRenderer(), 10, 10, 10, 255);
                break;
            }
            SDL_RenderFillRect(game->getRenderer(), &r);
            SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
            SDL_RenderDrawRect(game->getRenderer(), &r);
        }
    }
}
