#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <array>
#include <vector>
#include <span>
#include <string_view>

struct Level
{
    Level();
    ~Level();

    int rows = 6;
    int cols = 4;

    std::array<std::array<int, 10>, 10> mGrid = {{}};

    void clear();
    void set(int x, int y, int value);
    void load(const std::span<std::string_view>& ld);
};

extern std::array<std::span<std::string_view>, 2> LEVELS;

#endif