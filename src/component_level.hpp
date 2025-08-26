#ifndef LEVEL_COMPONENT_HPP
#define LEVEL_COMPONENT_HPP

#include "component_block.hpp"
#include "entity.hpp"
#include "level.hpp"

class BlockComponent;

class LevelComponent : public Component
{
  public:
    CTransform* transform;
    int lvlIdx;
    Level mModel;
    LevelVisualState mVisualState;
    Animation animRise;
    AnimationProperty<vec2> tileAnims[MAX_GRID_SIZE][MAX_GRID_SIZE];
    BlockComponent* block;

    std::function<void()> onLevelCleared = nullptr;

    LevelComponent(int lvlIdx, const LevelData& ld);

    void load(int lvlIdx, const LevelData& ld);

    void startRise(std::function<void()> onComplete);
    void init() override;
    void update(float dt) override;
    void draw() override;
};

#endif