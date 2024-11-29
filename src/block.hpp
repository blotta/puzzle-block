#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <utility>
#include "util.hpp"

enum BlockState
{
    UP,
    WIDE,
    LONG
};
struct Block
{
    int x;
    int y;
    BlockState state = BlockState::UP; 

    int prevX;
    int prevY;
    BlockState prevState = BlockState::UP; 

    void move(const vec2& dir);
    void undoMove();
    std::pair<vec2, vec2> getPositions();
};

#endif