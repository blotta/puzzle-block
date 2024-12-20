#include <stdio.h>
#include <string>

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
        {.id = SpriteID::BLOCK_WIDE,
         .tx = 160,
         .ty = 80,
         .tw = 96,
         .th = 80,
         .originY = 32,
         .originX = 0};

    data->Sprites[SpriteID::SWITCH] =
        {.id = SpriteID::SWITCH,
         .tx = 64,
         .ty = 160,
         .tw = 64,
         .th = 64,
         .originY = 0,
         .originX = 0};

    // levels
    data->DefaultLevels[0] =
    #include "./../assets/levels/0.txt"
    ;

    data->DefaultLevels[1] =
    #include "./../assets/levels/1.txt"
    ;

    data->DefaultLevels[2] =
    #include "./../assets/levels/2.txt"
    ;

    data->DefaultLevels[3] =
    #include "./../assets/levels/3.txt"
    ;

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
    char row[cols + 1] = {};
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
