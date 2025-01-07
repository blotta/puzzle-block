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
         .tx = 0,
         .ty = 64,
         .tw = 128,
         .th = 128,
         .originX = 32,
         .originY = 84
         };

    data->Sprites[SpriteID::SPR_BLOCK_LONG] =
        {.id = SpriteID::SPR_BLOCK_LONG,
         .tx = 128,
         .ty = 64,
         .tw = 128,
         .th = 128,
         .originX = 48,
         .originY = 76,
         };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE] =
        {.id = SpriteID::SPR_BLOCK_WIDE,
         .tx = 256,
         .ty = 64,
         .tw = 128,
         .th = 128,
         .originX = 16,
         .originY = 76,
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
