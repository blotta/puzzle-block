#ifndef DATA_HPP
#define DATA_HPP

#include <array>


// SPRITES

enum SpriteID
{
    FLOOR = 0,
    FLOOR_FINISH,
    FLOOR_START,
    FLOOR_HIGHLIGHT,
    BLOCK_UP,
    BLOCK_LONG,
    BLOCK_WIDE,
    SWITCH,

    NUM_SPRITES
};

struct Sprite
{
    SpriteID id;
    int tx = 0;
    int ty = 0;
    int tw = 0;
    int th = 0;
    int originY = 0;
    int originX = 0;
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

#define NUM_LEVELS 5
struct GameData
{
    Sprite Sprites[SpriteID::NUM_SPRITES];
    std::array<LevelData, NUM_LEVELS> DefaultLevels;
};

void load_game_data(GameData* d);

#endif