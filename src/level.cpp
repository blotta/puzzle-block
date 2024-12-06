#include <stdlib.h>
#include <span>

#include <SDL2/SDL.h>

#include "level.hpp"
#include "asset.hpp"

Level::Level(Game *game)
    : game(game)
{
}

Level::~Level()
{
}

void Level::clear()
{
    for (auto &row : grid)
    {
        for (auto &cell : row)
        {
            cell = CellType::EMPTY;
        }
    }
}

void Level::set(int x, int y, CellType value)
{
    grid[y][x] = value;
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

void Level::load(const LevelData &ld)
{
    this->clear();
    this->rows = ld.rows;
    this->cols = ld.cols;
    for (int row = 0; row < ld.rows; ++row)
    {
        for (int col = 0; col < ld.cols; ++col)
        {
            auto v = ld.data[row][col];
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
            if (grid[j][i] == CellType::START)
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
    if (!isValidPos(pos))
        return false;

    CellType type = grid[pos.y][pos.x];

    return type == CellType::FLOOR || type == CellType::START || type == CellType::FINISH;
}

void Level::draw(int x, int y, int cellSize)
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            if (grid[j][i] == CellType::EMPTY)
                continue;
            
            int sx;
            int sy;
            toISO(i, j, cellSize, cellSize / 2, &sx, &sy);

            SpriteID sprId = SpriteID::FLOOR;

            switch (grid[j][i])
            {
                case CellType::START:
                    sprId = SpriteID::FLOOR_START;
                    break;
                case CellType::FINISH:
                    sprId = SpriteID::FLOOR_FINISH;
                    break;
                default:
                    sprId = SpriteID::FLOOR;
                    break;
            }

            game->drawSprite(x + sx, y + sy, sprId);
        }
    }
}

void Level::toggleFloor(const vec2 &pos)
{
    CellType& cell = grid[pos.y][pos.x];
    switch (cell)
    {
        case CellType::EMPTY:
            cell = CellType::FLOOR;
            break;
        case CellType::FLOOR:
        case CellType::START:
        case CellType::FINISH:
            cell = CellType::EMPTY;
            break;
        default:
            break;
    }
}

void Level::toggleStartFinish(const vec2 &pos)
{
    CellType& cell = grid[pos.y][pos.x];
    switch (cell)
    {
        case CellType::START:
            cell = CellType::FINISH;
            break;
        case CellType::FINISH:
            cell = CellType::EMPTY;
            break;
        case CellType::EMPTY:
        case CellType::FLOOR:
            cell = CellType::START;
            break;
        default:
            break;
    }
}
