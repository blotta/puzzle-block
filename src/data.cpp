#include <stdio.h>
#include <string>

#include "data.hpp"

void load_game_data(GameData *data)
{
    data->Sprites[SpriteID::SPR_FLOOR] =
        {.id = SpriteID::SPR_FLOOR,
         .tx = 0,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originX = 0,
         .originY = 0,
         };

    data->Sprites[SpriteID::SPR_FLOOR_FINISH] =
        {.id = SpriteID::SPR_FLOOR_FINISH,
         .tx = 64,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originX = 0,
         .originY = 0,
         };

    data->Sprites[SpriteID::SPR_FLOOR_START] =
        {.id = SpriteID::SPR_FLOOR_START,
         .tx = 64 * 2,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originX = 0,
         .originY = 0,
         };

    data->Sprites[SpriteID::SPR_FLOOR_HIGHLIGHT] =
        {.id = SpriteID::SPR_FLOOR_HIGHLIGHT,
         .tx = 64 * 3,
         .ty = 0,
         .tw = 64,
         .th = 64,
         .originX = 0,
         .originY = 0,
         };

    data->Sprites[SpriteID::SPR_BLOCK_UP] =
        {.id = SpriteID::SPR_BLOCK_UP,
         .tx = 128 * 0,
         .ty = 64 + 128 * 0,
         .tw = 128,
         .th = 128,
         .originX = 32,
         .originY = 84
         };

    data->Sprites[SpriteID::SPR_BLOCK_UP_LONG_30] =
        {.id = SpriteID::SPR_BLOCK_UP_LONG_30,
         .tx = 128 * 1,
         .ty = 64 + 128 * 0,
         .tw = 128,
         .th = 128,
         .originX = 20,
         .originY = 90
         };

    data->Sprites[SpriteID::SPR_BLOCK_UP_LONG_60] =
        {.id = SpriteID::SPR_BLOCK_UP_LONG_60,
         .tx = 128 * 2,
         .ty = 64 + 128 * 0,
         .tw = 128,
         .th = 128,
         .originX = -3,
         .originY = 102
         };

    data->Sprites[SpriteID::SPR_BLOCK_LONG] =
        {.id = SpriteID::SPR_BLOCK_LONG,
         .tx = 128 * 3,
         .ty = 64 + 128 * 0,
         .tw = 128,
         .th = 128,
         .originX = 48,
         .originY = 76,
         };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_LONG_30] =
        {.id = SpriteID::SPR_BLOCK_LONG_LONG_30,
         .tx = 128 * 4,
         .ty = 64 + 128 * 0,
         .tw = 128,
         .th = 128,
         .originX = 41,
         .originY = 71,
         };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_LONG_60] =
        {.id = SpriteID::SPR_BLOCK_LONG_LONG_60,
         .tx = 128 * 0,
         .ty = 64 + 128 * 1,
         .tw = 128,
         .th = 128,
         .originX = 24,
         .originY = 64,
         };

    data->Sprites[SpriteID::SPR_BLOCK_UP_WIDE_30] =
        {.id = SpriteID::SPR_BLOCK_UP_WIDE_30,
         .tx = 128 * 1,
         .ty = 64 + 128 * 1,
         .tw = 128,
         .th = 128,
         .originX = 20,
         .originY = 78,
         };

    data->Sprites[SpriteID::SPR_BLOCK_UP_WIDE_60] =
        {.id = SpriteID::SPR_BLOCK_UP_WIDE_60,
         .tx = 128 * 2,
         .ty = 64 + 128 * 1,
         .tw = 128,
         .th = 128,
         .originX = -2,
         .originY = 67,
         };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE] =
        {.id = SpriteID::SPR_BLOCK_WIDE,
         .tx = 128 * 3,
         .ty = 64 + 128 * 1,
         .tw = 128,
         .th = 128,
         .originX = 16,
         .originY = 76,
         };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_WIDE_30] =
        {.id = SpriteID::SPR_BLOCK_WIDE_WIDE_30,
         .tx = 128 * 4,
         .ty = 64 + 128 * 1,
         .tw = 128,
         .th = 128,
         .originX = 8,
         .originY = 80,
         };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_WIDE_60] =
        {.id = SpriteID::SPR_BLOCK_WIDE_WIDE_60,
         .tx = 128 * 0,
         .ty = 64 + 128 * 2,
         .tw = 128,
         .th = 128,
         .originX = -8,
         .originY = 87,
         };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_UP_30] =
        {.id = SpriteID::SPR_BLOCK_LONG_UP_30,
         .tx = 128 * 1,
         .ty = 64 + 128 * 2,
         .tw = 128,
         .th = 128,
         .originX = 68,
         .originY = 66,
         };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_UP_60] =
        {.id = SpriteID::SPR_BLOCK_LONG_UP_60,
         .tx = 128 * 2,
         .ty = 64 + 128 * 2,
         .tw = 128,
         .th = 128,
         .originX = 45,
         .originY = 78,
         };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_UP_30] =
        {.id = SpriteID::SPR_BLOCK_WIDE_UP_30,
         .tx = 128 * 3,
         .ty = 64 + 128 * 2,
         .tw = 128,
         .th = 128,
         .originX = 67,
         .originY = 102,
         };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_UP_60] =
        {.id = SpriteID::SPR_BLOCK_WIDE_UP_60,
         .tx = 128 * 4,
         .ty = 64 + 128 * 2,
         .tw = 128,
         .th = 128,
         .originX = 44,
         .originY = 90,
         };

    data->Sprites[SpriteID::SPR_SWITCH_OFF] =
        {.id = SpriteID::SPR_SWITCH_OFF,
         .tx = 320,
         .ty = 0,
         .tw = 32,
         .th = 32,
         .originX = -16,
         .originY = 0,
         };

    data->Sprites[SpriteID::SPR_SWITCH_ON] =
        {.id = SpriteID::SPR_SWITCH_ON,
         .tx = 352,
         .ty = 0,
         .tw = 32,
         .th = 32,
         .originX = -16,
         .originY = 0,
         };

    // levels
    #include "./../assets/levels/gen_level_data.cpp.part"
}

void LevelData::print(FILE *file) const
{
    FILE* f = file;
    bool needClose = false;
    if (f == nullptr)
    {
        f = fopen("level.txt", "w");
        needClose = true;
    }

    fprintf(stderr, "===LevelData===\n");
    fprintf(f, "{\n");
    fprintf(f, "    .cols = %d,\n", cols);
    fprintf(f, "    .rows = %d,\n", rows);

    // data
    fprintf(f, "    .data = {\n");
    char row[MAX_GRID_SIZE] = {};
    for (int j = 0; j < rows; j++)
    {
        int p = j * cols;
        memcpy(&row, &data[p], cols);
        fprintf(f, "        \"%s\"\n", row);
    }

    if (switchCount > 0)
        fprintf(f, "    },\n");
    else
        fprintf(f, "    }\n");

    // switches
    if (switchCount > 0)
    {
        fprintf(f, "    .switchCount = %d,\n", switchCount);
        fprintf(f, "    .switches = {\n");
        for (int i = 0; i < switchCount; i++)
        {
            auto& sw = switches[i];
            std::string swtype = "SINGLE";
            if (sw.type == LevelSwitchType::DOUBLE)
                swtype = "DOUBLE";

            fprintf(f, "        {.x = %d, .y = %d, .type = LevelSwitchType::%s, .floorX = %d, .floorY = %d}"
                , sw.x, sw.y, swtype.c_str(), sw.floorX, sw.floorY);
            
            if (i + 1 < switchCount)
                fprintf(f, ",\n");
            else
                fprintf(f, "\n");
        }
        fprintf(f, "    }\n");
    }
    fprintf(f, "}\n");

    if (needClose)
        fclose(f);
}
