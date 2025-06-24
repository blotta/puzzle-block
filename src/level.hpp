#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <array>
#include <vector>
#include <span>
#include <SDL2/SDL.h>

#include "game.hpp"
#include "util.hpp"
#include "data.hpp"


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
struct Level
{
    int rows = 6;
    int cols = 4;

    std::array<std::array<CellType, MAX_GRID_SIZE>, MAX_GRID_SIZE> grid = {{}};
    int switchCount = 0;
    LevelSwitch switches[MAX_SWITCH_COUNT] = {};

    void clear();
    void set(int x, int y, CellType value);
    void load(const LevelData &ld);
    void toLevelData(LevelData* ld);
    vec2 getStartPos();
    bool isValidPos(const vec2 &pos) const;
    bool isValid();
    bool hasFloorAt(const vec2 &pos) const;
    bool hasSwitchAt(const vec2 &pos, LevelSwitch** sw);
    bool checkAndTriggerSwitches(const vec2 &pos1, const vec2 &pos2);
    void toggleThinFloor(const vec2 &pos);
    CellType cellAt(const vec2 &pos);
    void draw(int x, int y, int cellSize);

    void toggleFloor(const vec2& pos);
    void toggleSpecialFloor(const vec2& pos);
    void addRow();
    void removeRow();
    void addColumn();
    void removeColumn();
    void addSwitch(LevelSwitch sw);
    void removeSwitch(const vec2& pos);
};

#endif