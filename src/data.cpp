
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

    data->Sprites[SpriteID::SWITCH] =
        {
            .id = SpriteID::SWITCH,
            .tx = 64,
            .ty = 160,
            .tw = 64,
            .th = 64,
            .originY = 0,
            .originX = 0};

    // levels
    data->DefaultLevels[0] =
        {
            .cols = 2,
            .rows = 7,
            .data = {
                "03"
                "11"
                "11"
                "10"
                "10"
                "10"
                "20"
            },
            .switchCount = 1,
            .switches = {
                0, 1, LevelSwitchType::SINGLE, 0, 0
            }
        };

    data->DefaultLevels[1] =
        {
            .cols = 4,
            .rows = 6,
            .data = {
                "3111"
                "0001"
                "0111"
                "0111"
                "0001"
                "2111"
            }

        };

    data->DefaultLevels[2] =
        {
            .cols = 4,
            .rows = 9,
            .data = {
                "3111"
                "0001"
                "0111"
                "0111"
                "0001"
                "0111"
                "0111"
                "0001"
                "2111"
            }
        };

    data->DefaultLevels[3] =
        {
            .cols = 4,
            .rows = 5,
            .data = {
                "0030"
                "0111"
                "0111"
                "0111"
                "2111"
            }
        };
}