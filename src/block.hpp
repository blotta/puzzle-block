#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <SDL2/SDL.h>

#include <utility>
#include "game.hpp"
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

    void draw(int levelX, int levelY, int cellSize);
};


#endif