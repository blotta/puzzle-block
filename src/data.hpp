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

struct LevelData
{
    int rows;
    int cols;
    char data[10][11];
};

// Game data context

struct GameData
{
    Sprite Sprites[SpriteID::NUM_SPRITES];
    std::array<LevelData, 3> DefaultLevels;
};

void load_game_data(GameData* d);

#endif