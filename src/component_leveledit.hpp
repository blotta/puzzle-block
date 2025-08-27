#ifndef LEVELEDIT_COMPONENT_HPP
#define LEVELEDIT_COMPONENT_HPP

#include "component_block.hpp"
#include "entity.hpp"
#include "level.hpp"

class BlockComponent;

class LevelEditComponent : public Component
{
  public:
    CTransform* transform;
    int lvlIdx;
    LevelEditComponent(int lvlIdx, const LevelData& ld);

    void load(int lvlIdx, const LevelData& ld);
    void save(bool newLevel, bool saveToFile);
    int cols() const;
    int rows() const;
    bool isModified() const;
    bool isValid() const;
    vec2 getStartPos() const;
    void toggleFloor(const vec2& pos);
    void toggleSpecialFloor(const vec2& pos);
    bool isValidPos(const vec2& pos) const;
    CellType cellAt(const vec2& pos) const;
    bool hasSwitchAt(const vec2& pos, LevelSwitch** sw);
    void removeSwitch(const vec2& pos);
    void addSwitch(LevelSwitch sw);

    void init() override;
    void update(float dt) override;
    void draw() override;

private:
    Level mModel;
    bool modified = false;
    bool justModified = false;
    Rect rect;
};

#endif