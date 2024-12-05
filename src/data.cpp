
#include "data.hpp"

Sprite SPRITES[SpriteID::NUM_SPRITES] = {
    {.id = SpriteID::FLOOR,
     .tx = 0,
     .ty = 0,
     .tw = 64,
     .th = 64,
     .originY = 0,
     .originX = 0},

    {.id = SpriteID::FLOOR_FINISH,
     .tx = 64,
     .ty = 0,
     .tw = 64,
     .th = 64,
     .originY = 0,
     .originX = 0},

    {.id = SpriteID::FLOOR_START,
     .tx = 64 * 2,
     .ty = 0,
     .tw = 64,
     .th = 64,
     .originY = 0,
     .originX = 0},

    {.id = SpriteID::FLOOR_HIGHLIGHT,
     .tx = 64 * 3,
     .ty = 0,
     .tw = 64,
     .th = 64,
     .originY = 0,
     .originX = 0},

    {.id = SpriteID::BLOCK_UP,
     .tx = 0,
     .ty = 64,
     .tw = 64,
     .th = 96,
     .originY = 64,
     .originX = 0},

    {.id = SpriteID::BLOCK_LONG,
     .tx = 64,
     .ty = 80,
     .tw = 96,
     .th = 80,
     .originY = 32,
     .originX = 32},

    {.id = SpriteID::BLOCK_WIDE,
     .tx = 160,
     .ty = 80,
     .tw = 96,
     .th = 80,
     .originY = 32,
     .originX = 0}};
    


std::array<LevelData, 3> DEFAULT_LEVELS = {{

    {.rows = 10, .cols = 10, .data = {

                                 "1111113111",
                                 "1111110001",
                                 "1111110111",
                                 "1111110111",
                                 "1111110001",
                                 "1111112111",
                                 "1111110111",
                                 "1111110111",
                                 "1111110111",
                                 "1111110111",
                             }

    },
    {.rows = 6, .cols = 4, .data = {
                               "3111",
                               "0001",
                               "0111",
                               "0111",
                               "0001",
                               "2111",
                           }

    },
    {.rows = 6, .cols = 4, .data = {
                               "1311",
                               "0001",
                               "0111",
                               "0111",
                               "0001",
                               "2111",
                           }

    },
}};
