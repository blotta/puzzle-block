#ifndef DATA_HPP
#define DATA_HPP

#include <array>
#include <cstdio>

// SPRITES

enum SpriteID
{
    SPR_FLOOR = 0,
    SPR_FLOOR_FINISH,
    SPR_FLOOR_START,
    SPR_FLOOR_THIN,
    SPR_FLOOR_HIGHLIGHT,

    SPR_BLOCK_UP,
    SPR_BLOCK_UP_LONG_30,
    SPR_BLOCK_UP_LONG_60,
    SPR_BLOCK_LONG,
    SPR_BLOCK_LONG_LONG_30,
    SPR_BLOCK_LONG_LONG_60,
    SPR_BLOCK_UP_WIDE_30,
    SPR_BLOCK_UP_WIDE_60,
    SPR_BLOCK_WIDE,
    SPR_BLOCK_WIDE_WIDE_30,
    SPR_BLOCK_WIDE_WIDE_60,
    SPR_BLOCK_LONG_UP_30,
    SPR_BLOCK_LONG_UP_60,
    SPR_BLOCK_WIDE_UP_30,
    SPR_BLOCK_WIDE_UP_60,

    SPR_SWITCH_OFF,
    SPR_SWITCH_ON,

    SPR_TITLE,

    SPR_NUM_SPRITES
};

struct Sprite
{
    SpriteID id;
    int tx = 0;
    int ty = 0;
    int tw = 0;
    int th = 0;
    int originX = 0;
    int originY = 0;
};

// LEVELS

const int MAX_GRID_SIZE = 15;
const int MAX_SWITCH_COUNT = 2;

enum class LevelSwitchType
{
    SINGLE = 0,
    DOUBLE
};

struct LevelSwitch
{
    int x;
    int y;
    LevelSwitchType type;
    int floorX;
    int floorY;
    bool on = false;
};

struct LevelData
{
    int cols;
    int rows;
    char data[MAX_GRID_SIZE * MAX_GRID_SIZE + 1];
    int switchCount = 0;
    LevelSwitch switches[MAX_SWITCH_COUNT];

    void print(FILE* f) const;
};

// Game data context

#include "./../assets-build/levels/gen_level_data.hpp"

struct GameSettings
{
    int sfx_vol = 5;
    int music_vol = 5;
};

struct GameData
{
    Sprite Sprites[SpriteID::SPR_NUM_SPRITES];
    std::array<LevelData, NUM_LEVELS> DefaultLevels;
};

void load_game_data(GameData* d);

LevelData generateRandomLevel(int pathLength);

#endif