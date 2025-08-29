#ifndef BLOCK_COMPONENT_HPP
#define BLOCK_COMPONENT_HPP

#include "block.hpp"
#include "component_level.hpp"
#include "entity.hpp"

class LevelComponent;

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

class BlockComponent : public Component
{
  public:
    CTransform* transform = nullptr;
    LevelComponent* level = nullptr;

    BlockSim currSim, nextSim;
    BlockVisualState vState = BlockVisualState::IDLE;

    Animation anim;
    AnimationProperty<SpriteID> animProp;

    Animation animFallStart;
    AnimationProperty<float> animFallStartProp;

    Animation animFlyEnd;
    AnimationProperty<float> animFlyEndProp;

    ParticleSystem particleSystem;

    bool moved = false;
    bool hitFloor = false;

    BlockVisualTransitionData transitionData;

    void startFall();
    void startFall(std::function<void()> onComplete);
    void startFly(std::function<void()> onComplete);
    void init() override;
    void update(float dt) override;
    void draw() override;
    static BlockVisualTransition getTransition(const BlockSim& curr, const BlockSim& next);
};

#endif