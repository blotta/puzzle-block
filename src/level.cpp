#include "level.hpp"
#include <stdlib.h>
#include <span>

#include <SDL2/SDL.h>

static IsoSprite isoSprFloor = {
    .tx = 0,
    .ty = 0,
    .tw = 64,
    .th = 64,
    .heightOffset = 0,
    .widthOffset = 0};

static IsoSprite isoSprFinish = {
    .tx = 64,
    .ty = 0,
    .tw = 64,
    .th = 64,
    .heightOffset = 0,
    .widthOffset = 0};

static IsoSprite isoSprStart = {
    .tx = 64 * 2,
    .ty = 0,
    .tw = 64,
    .th = 64,
    .heightOffset = 0,
    .widthOffset = 0};

Level::Level(Game *game)
    : game(game)
{
    pSpriteSheet = game->getTexture("spritesheet");
}

Level::~Level()
{
}

void Level::clear()
{
    for (auto &row : mGrid)
    {
        for (auto &cell : row)
        {
            cell = CellType::EMPTY;
        }
    }
}

void Level::set(int x, int y, CellType value)
{
    mGrid[y][x] = value;
}

void Level::load(const std::span<std::string_view> &ld)
{
    this->clear();
    this->rows = ld.size();
    this->cols = ld[0].size();
    for (size_t row = 0; row < ld.size(); ++row)
    {
        for (size_t col = 0; col < ld[row].size(); ++col)
        {
            auto v = ld[row][col];
            CellType vi = (CellType)atoi(&v);
            this->set(col, row, vi);
        }
    }
}

vec2 Level::getStartPos()
{
    for (int j = 0; j < this->rows; j++)
    {
        for (int i = 0; i < this->cols; i++)
        {
            if (mGrid[j][i] == CellType::START)
                return vec2(i, j);
        }
    }

    return vec2();
}

bool Level::isValidPos(const vec2 &pos)
{
    return pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows;
}

bool Level::hasFloorAt(const vec2 &pos)
{
    CellType type = mGrid[pos.y][pos.x];

    return type == CellType::FLOOR || type == CellType::START || type == CellType::FINISH;
}

void Level::draw(SDL_Renderer *rend, int x, int y, int cellSize)
{
    SDL_Rect rr = {x - 1, y - 1, cellSize * this->cols + 2, cellSize * this->rows + 2};
    SDL_SetRenderDrawColor(rend, 40, 200, 80, 255);
    SDL_RenderDrawRect(rend, &rr);

    // draw level
    for (int j = 0; j < rows; j++)
    {
        for (int i = 0; i < cols; i++)
        {
            SDL_Rect r = {
                x + cellSize * i,
                y + cellSize * j,
                cellSize,
                cellSize};

            switch (mGrid[j][i])
            {
            case CellType::FLOOR: // floor
                SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
                break;
            case CellType::START: // start
                SDL_SetRenderDrawColor(rend, 100, 100, 200, 255);
                break;
            case CellType::FINISH: // finish
                SDL_SetRenderDrawColor(rend, 100, 200, 100, 255);
                break;
            case CellType::EMPTY:
            default: // empty
                SDL_SetRenderDrawColor(rend, 10, 10, 10, 255);
                break;
            }
            SDL_RenderFillRect(rend, &r);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderDrawRect(rend, &r);
        }
    }
}

void Level::drawISO(SDL_Renderer *rend, int x, int y, int cellSize)
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            IsoSprite *spr = nullptr;

            switch (mGrid[j][i])
            {
            case CellType::START: // start
                spr = &isoSprStart;
                break;
            case CellType::FINISH: // finish
                spr = &isoSprFinish;
                break;
            case CellType::FLOOR: // floor
                spr = &isoSprFloor;
                break;
            case CellType::EMPTY:
            default: // empty
                break;
            }

            if (spr != nullptr)
            {
                int sx;
                int sy;
                toISO(i, j, cellSize, cellSize / 2, &sx, &sy);
                SDL_Rect dest = {
                    x + sx,
                    y + sy,
                    cellSize,
                    cellSize};

                SDL_Rect src = {spr->tx, spr->ty, spr->tw, spr->th};
                SDL_RenderCopy(rend, pSpriteSheet, &src, &dest);
            }
        }
    }
}

std::array<std::string_view, 6> LVL01 = {
    "3111",
    "0001",
    "0111",
    "0111",
    "0001",
    "2111",
};

std::vector<std::string_view> LVL02 = {
    "3111",
    "0111",
    "1001",
    "0001",
    "0011",
    "2111",
};

std::array<std::span<std::string_view>, 2> LEVELS = {
    LVL01,
    LVL02};