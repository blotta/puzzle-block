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

struct BlockSim
{
    int x;
    int y;
    BlockState state = BlockState::UP;

    void move(const vec2& dir);
    std::pair<vec2, vec2> getPositions();

    static std::pair<vec2, vec2> GetBlockPositionsForState(const vec2& pos, BlockState state);
};

enum class BlockVisualState
{
    IDLE,
    MOVING,
    FALLING_LEVEL_START,
    FLYING_LEVEL_END,
    INVISIBLE,
};

enum class BlockVisualTransition
{
    IDLE_UP,
    IDLE_WIDE,
    IDLE_LONG,
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

struct BlockVisualTransitionData
{
    std::array<SpriteID, 2> frames;
    bool useCurrSim = true;
};

struct BlockVisual
{
    BlockSim currSim, nextSim;
    BlockVisualState vState = BlockVisualState::IDLE;
    Animation anim;
    AnimationProperty<SpriteID> animProp;
    BlockVisualTransitionData transitionData;
    Animation animFallStart;
    AnimationProperty<float> animFallStartProp;
    Animation animFlyEnd;
    AnimationProperty<float> animFlyEndProp;
    Level* level = nullptr;
    bool moved = false;
    void init(const vec2& pos, BlockState state);
    void update(float dt);
    void draw(int levelX, int levelY, int cellSize);
    void startFall();
    void startFall(const std::function<void()>& onComplete);
    void startFly(const std::function<void()>& onComplete);
    static BlockVisualTransition getTransition(const BlockSim& curr, const BlockSim& next);
};

#endif