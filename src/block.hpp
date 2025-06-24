#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <SDL2/SDL.h>

#include "animation.hpp"
#include "game.hpp"
#include "level.hpp"
#include "util.hpp"
#include <utility>

enum BlockState
{
    UP,
    WIDE,
    LONG
};

enum class BlockTransition
{
    IDLE,
    UP_LONG_FORWARD,
    UP_LONG_BACKWARD,
    LONG_UP_FORWARD,
    LONG_UP_BACKWARD,
    UP_WIDE_RIGHT,
    UP_WIDE_LEFT,
    WIDE_UP_RIGHT,
    WIDE_UP_LEFT,
    LONG_LONG_RIGHT,
    LONG_LONG_LEFT,
    WIDE_WIDE_FORWARD,
    WIDE_WIDE_BACKWARD
};

struct BlockMovement
{
    vec2 oldPos, newPos;
    BlockState oldState, newState;
    bool moved = false;
    BlockTransition transition;
};

struct Block
{
    int x;
    int y;
    BlockState state = BlockState::UP;

    BlockTransition currTransition = BlockTransition::IDLE;
    AnimationSprite* animation;
    BlockMovement moveIntent;

    static std::pair<vec2, vec2> GetBlockPositionsForState(const vec2& pos, BlockState state);

    void init(const vec2& pos, BlockState state);
    void updateMovementIntent(const vec2& dir);
    bool move(const vec2& dir, const Level& level, bool collide);
    std::pair<vec2, vec2> getPositions();

    void draw(int levelX, int levelY, int cellSize);
};

struct BlockSim
{
    int x;
    int y;
    BlockState state = BlockState::UP;

    void move(const vec2& dir);
    std::pair<vec2, vec2> getPositions();
};

#endif