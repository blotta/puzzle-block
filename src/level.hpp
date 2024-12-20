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
    GHOST = 4,
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
    bool isValidPos(const vec2 &pos);
    bool isValid();
    bool hasFloorAt(const vec2 &pos);
    bool hasSwitchAt(const vec2 &pos, LevelSwitch** sw);
    void toggleGhostFloor(const vec2 &pos);
    CellType cellAt(const vec2 &pos);
    void draw(int x, int y, int cellSize);

    void toggleFloor(const vec2& pos);
    void toggleStartFinish(const vec2& pos);
    void addRow();
    void removeRow();
    void addColumn();
    void removeColumn();
    void addSwitch(LevelSwitch sw);
    void removeSwitch(const vec2& pos);
};

#endif