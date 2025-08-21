#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "animation.hpp"
#include "data.hpp"
#include "game.hpp"
#include "log.hpp"
#include "util.hpp"
#include <SDL2/SDL.h>
#include <array>
#include <span>
#include <vector>

class Game;
struct LevelData;

enum class CellType
{
    EMPTY = 0,
    FLOOR = 1,
    START = 2,
    FINISH = 3,
    THIN = 4,
};

enum class LevelVisualState
{
    IDLE,
    RISING
};

struct Level
{
    int rows = 6;
    int cols = 4;

    std::array<std::array<CellType, MAX_GRID_SIZE>, MAX_GRID_SIZE> grid = {{}};
    int switchCount = 0;
    LevelSwitch switches[MAX_SWITCH_COUNT] = {};

    void clear();
    void set(int x, int y, CellType value);
    void load(const LevelData& ld);
    void toLevelData(LevelData* ld);
    vec2 getStartPos();
    bool isValidPos(const vec2& pos) const;
    bool isValid();
    bool hasFloorAt(const vec2& pos) const;
    bool hasSwitchAt(const vec2& pos, LevelSwitch** sw);
    bool checkAndTriggerSwitches(const vec2& pos1, const vec2& pos2);
    bool isCleared(const vec2& pos1, const vec2& pos2);
    void toggleThinFloor(const vec2& pos);
    CellType cellAt(const vec2& pos);
    Rect rect() const;

    void toggleFloor(const vec2& pos);
    void toggleSpecialFloor(const vec2& pos);
    void addRow();
    void removeRow();
    void addColumn();
    void removeColumn();
    void addSwitch(LevelSwitch sw);
    void removeSwitch(const vec2& pos);
    void trim();
    void moveCellsBy(const vec2& diff);
    void centerCells();
};

struct LevelVisual
{
    vec2f pos;
    Level mModel;
    LevelVisualState mVisualState;
    Animation animRise;

    AnimationProperty<vec2> tileAnims[MAX_GRID_SIZE][MAX_GRID_SIZE];
    void startRise(std::function<void()> onComplete);

    void init(const LevelData& ld);
    void update(float dt);
    void draw();
};

#endif