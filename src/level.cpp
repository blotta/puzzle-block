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

void printLevel(const LevelData &lvl)
{
    SDL_Log("===LevelData===\n");
    SDL_Log("rows: %d    cols:%d\n", lvl.rows, lvl.cols);
    SDL_Log("data:\n");
    char row[MAX_GRID_SIZE + 1] = {};
    for (int j = 0; j < lvl.rows; j++)
    {
        memcpy(&row, lvl.data[j], lvl.cols);
        SDL_Log("%s\n", row);
    }
}

void Level::toLevelData(LevelData *ld)
{
    ld->cols = cols;
    ld->rows = rows;

    for (int j = 0; j < MAX_GRID_SIZE; j++)
    {
        for (int i = 0; i < MAX_GRID_SIZE; i++)
        {
            if (j >= this->rows || i >= this->cols)
            {
                ld->data[j][i] = '0';
                continue;
            }

            switch (grid[j][i])
            {
            case CellType::EMPTY:
                ld->data[j][i] = '0';
                break;
            case CellType::FLOOR:
                ld->data[j][i] = '1';
                break;
            case CellType::START:
                ld->data[j][i] = '2';
                break;
            case CellType::FINISH:
                ld->data[j][i] = '3';
                break;
            }
        }
    }
    printLevel(*ld);
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

bool Level::isValid()
{
    int startCount = 0;
    int finishCount = 0;
    bool valid = true;

    for (int j = 0; j < MAX_GRID_SIZE; j++)
    {
        for (int i = 0; i < MAX_GRID_SIZE; i++)
        {
            // if (j >= rows && grid[j][i] != CellType::EMPTY)
            //     valid = false;

            // if (i >= cols && grid[j][i] != CellType::EMPTY)
            //     valid = false;

            if (j < rows && i < cols && grid[j][i] == CellType::START)
                startCount += 1;

            if (j < rows && i < cols && grid[j][i] == CellType::FINISH)
                finishCount += 1;
        }
    }

    if (startCount != 1 || finishCount != 1)
        valid = false;

    return valid;
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
    CellType &cell = grid[pos.y][pos.x];
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
    CellType &cell = grid[pos.y][pos.x];
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

void Level::addRow()
{
    if (rows < MAX_GRID_SIZE)
        rows++;
}

void Level::removeRow()
{
    if (rows > 1)
        rows--;
}

void Level::addColumn()
{
    if (cols < MAX_GRID_SIZE)
        cols++;
}

void Level::removeColumn()
{
    if (cols > 1)
        cols--;
}
