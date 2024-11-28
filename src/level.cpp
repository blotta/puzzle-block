#include "level.hpp"
#include <stdlib.h>
#include <span>

Level::Level()
{
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

std::array<std::string_view, 6> LVL01 = {
    "3111",
    "0001",
    "0001",
    "0001",
    "0001",
    "2111",
};

std::vector<std::string_view> LVL02 = {
    "3111",
    "0001",
    "1001",
    "0001",
    "0011",
    "2111",
};

std::array<std::span<std::string_view>, 2> LEVELS = {
    LVL01,
    LVL02
};