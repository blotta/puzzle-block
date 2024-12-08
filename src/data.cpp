
#include "data.hpp"

void load_game_data(GameData *data)
{
    data->Sprites[SpriteID::FLOOR] =
        {.id = SpriteID::FLOOR,
         .tx = 0,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originY = 0,
         .originX = 0};

    data->Sprites[SpriteID::FLOOR_FINISH] =
        {.id = SpriteID::FLOOR_FINISH,
         .tx = 64,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originY = 0,
         .originX = 0};

    data->Sprites[SpriteID::FLOOR_START] =
        {.id = SpriteID::FLOOR_START,
         .tx = 64 * 2,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originY = 0,
         .originX = 0};

    data->Sprites[SpriteID::FLOOR_HIGHLIGHT] =
        {.id = SpriteID::FLOOR_HIGHLIGHT,
         .tx = 64 * 3,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originY = 0,
         .originX = 0};

    data->Sprites[SpriteID::BLOCK_UP] =
        {.id = SpriteID::BLOCK_UP,
         .tx = 0,
         .ty = 64,
         .tw = 64,
         .th = 96,
         .originY = 64,
         .originX = 0};

    data->Sprites[SpriteID::BLOCK_LONG] =
        {.id = SpriteID::BLOCK_LONG,
         .tx = 64,
         .ty = 80,
         .tw = 96,
         .th = 80,
         .originY = 32,
         .originX = 32};

    data->Sprites[SpriteID::BLOCK_WIDE] =
        {
            .id = SpriteID::BLOCK_WIDE,
            .tx = 160,
            .ty = 80,
            .tw = 96,
            .th = 80,
            .originY = 32,
            .originX = 0};

    // levels
    data->DefaultLevels[0] =
        {
            .rows = 7,
            .cols = 1,
            .data = {
                "3",
                "1",
                "1",
                "1",
                "1",
                "1",
                "2",
            }};

    data->DefaultLevels[1] =
        {
            .rows = 10,
            .cols = 10,
            .data = {
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
            }};

    data->DefaultLevels[2] =
        {
            .rows = 6,
            .cols = 4,
            .data = {
                "3111",
                "0001",
                "0111",
                "0111",
                "0001",
                "2111",
            }

        };

    data->DefaultLevels[3] =
        {
            .rows = 6,
            .cols = 4,
            .data = {
                "1311",
                "0001",
                "0111",
                "0111",
                "0001",
                "2111",
            }

        };
}