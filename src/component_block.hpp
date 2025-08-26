#ifndef BLOCK_COMPONENT_HPP
#define BLOCK_COMPONENT_HPP

#include "block.hpp"
#include "component_level.hpp"
#include "entity.hpp"

class LevelComponent;

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
};

#endif