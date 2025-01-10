#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <SDL2/SDL.h>

#include <utility>
#include "game.hpp"
#include "util.hpp"
#include "level.hpp"

enum BlockState
{
    UP,
    WIDE,
    LONG
};

enum class BlockAnimationState
{
    IDLE,
    MOVING
};

struct Block
{
    int x;
    int y;
    BlockState state = BlockState::UP; 
    BlockAnimationState animState = BlockAnimationState::IDLE; 

    bool move(const vec2& dir, const Level& level, bool collide);
    std::pair<vec2, vec2> getPositions();

    void draw(int levelX, int levelY, int cellSize);
};


#endif