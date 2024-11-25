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
            cell = 0;
        }
    }
}

void Level::set(int x, int y, int value)
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
            this->set(col, row, atoi(&v));
        }
    }
}

std::array<std::string_view, 6> LVL01 = {
    "1111",
    "0001",
    "0001",
    "0001",
    "0001",
    "1111",
};

std::vector<std::string_view> LVL02 = {
    "1111",
    "0001",
    "1001",
    "0001",
    "0011",
    "1111",
};

std::array<std::span<std::string_view>, 2> LEVELS = {
    LVL01,
    LVL02
};