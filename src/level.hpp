#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <array>
#include <vector>
#include <span>
#include <string_view>

#include "util.hpp"


enum CellType
{
    EMPTY = 0,
    FLOOR = 1,
    START = 2,
    FINISH = 3
};
struct Level
{
    Level();
    ~Level();

    int rows = 6;
    int cols = 4;

    std::array<std::array<CellType, 10>, 10> mGrid = {{}};

    void clear();
    void set(int x, int y, CellType value);
    void load(const std::span<std::string_view>& ld);
    vec2 getStartPos();
};

extern std::array<std::span<std::string_view>, 2> LEVELS;

#endif