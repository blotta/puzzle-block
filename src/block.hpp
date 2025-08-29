#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "animation.hpp"
#include "game.hpp"
#include "level.hpp"
#include "particle.hpp"
#include "util.hpp"
#include <SDL2/SDL.h>
#include <utility>

enum BlockState
{
    UP,
    WIDE,
    LONG
};

struct BlockSim
{
    int x;
    int y;
    BlockState state = BlockState::UP;

    void move(const vec2& dir);
    std::pair<vec2, vec2> getPositions();

    static std::pair<vec2, vec2> GetBlockPositionsForState(const vec2& pos, BlockState state);
};

#endif