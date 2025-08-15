#include <algorithm>
#include <cstring>
#include <random>
#include <stdio.h>
#include <string>
#include <vector>

#include "block.hpp"
#include "data.hpp"

void load_game_data(GameData* data)
{
    data->Sprites[SpriteID::SPR_FLOOR] = {
        .id = SpriteID::SPR_FLOOR,
        .tx = 0,
        .ty = 0,
        .tw = 64,
        .th = 64,
        .originX = 0,
        .originY = 0,
    };

    data->Sprites[SpriteID::SPR_FLOOR_FINISH] = {
        .id = SpriteID::SPR_FLOOR_FINISH,
        .tx = 64,
        .ty = 0,
        .tw = 64,
        .th = 64,
        .originX = 0,
        .originY = 0,
    };

    data->Sprites[SpriteID::SPR_FLOOR_START] = {
        .id = SpriteID::SPR_FLOOR_START,
        .tx = 64 * 2,
        .ty = 0,
        .tw = 64,
        .th = 64,
        .originX = 0,
        .originY = 0,
    };

    data->Sprites[SpriteID::SPR_FLOOR_THIN] = {
        .id = SpriteID::SPR_FLOOR_THIN,
        .tx = 64 * 3,
        .ty = 0,
        .tw = 64,
        .th = 64,
        .originX = 0,
        .originY = 0,
    };

    data->Sprites[SpriteID::SPR_FLOOR_HIGHLIGHT] = {
        .id = SpriteID::SPR_FLOOR_HIGHLIGHT,
        .tx = 64 * 6,
        .ty = 0,
        .tw = 64,
        .th = 64,
        .originX = 0,
        .originY = 0,
    };

    data->Sprites[SpriteID::SPR_SWITCH_OFF] = {
        .id = SpriteID::SPR_SWITCH_OFF,
        .tx = 512 + 32 * 0,
        .ty = 0,
        .tw = 32,
        .th = 32,
        .originX = -16,
        .originY = 0,
    };

    data->Sprites[SpriteID::SPR_SWITCH_ON] = {
        .id = SpriteID::SPR_SWITCH_ON,
        .tx = 512 + 32 * 1,
        .ty = 0,
        .tw = 32,
        .th = 32,
        .originX = -16,
        .originY = 0,
    };

    data->Sprites[SpriteID::SPR_BLOCK_UP] = {.id = SpriteID::SPR_BLOCK_UP,
                                             .tx = 128 * 0,
                                             .ty = 64 + 128 * 0,
                                             .tw = 128,
                                             .th = 128,
                                             .originX = 32,
                                             .originY = 84};

    data->Sprites[SpriteID::SPR_BLOCK_UP_LONG_30] = {.id = SpriteID::SPR_BLOCK_UP_LONG_30,
                                                     .tx = 128 * 1,
                                                     .ty = 64 + 128 * 0,
                                                     .tw = 128,
                                                     .th = 128,
                                                     .originX = 20,
                                                     .originY = 90};

    data->Sprites[SpriteID::SPR_BLOCK_UP_LONG_60] = {.id = SpriteID::SPR_BLOCK_UP_LONG_60,
                                                     .tx = 128 * 2,
                                                     .ty = 64 + 128 * 0,
                                                     .tw = 128,
                                                     .th = 128,
                                                     .originX = -3,
                                                     .originY = 102};

    data->Sprites[SpriteID::SPR_BLOCK_LONG] = {
        .id = SpriteID::SPR_BLOCK_LONG,
        .tx = 128 * 3,
        .ty = 64 + 128 * 0,
        .tw = 128,
        .th = 128,
        .originX = 48,
        .originY = 76,
    };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_LONG_30] = {
        .id = SpriteID::SPR_BLOCK_LONG_LONG_30,
        .tx = 128 * 4,
        .ty = 64 + 128 * 0,
        .tw = 128,
        .th = 128,
        .originX = 41,
        .originY = 71,
    };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_LONG_60] = {
        .id = SpriteID::SPR_BLOCK_LONG_LONG_60,
        .tx = 128 * 0,
        .ty = 64 + 128 * 1,
        .tw = 128,
        .th = 128,
        .originX = 24,
        .originY = 64,
    };

    data->Sprites[SpriteID::SPR_BLOCK_UP_WIDE_30] = {
        .id = SpriteID::SPR_BLOCK_UP_WIDE_30,
        .tx = 128 * 1,
        .ty = 64 + 128 * 1,
        .tw = 128,
        .th = 128,
        .originX = 20,
        .originY = 78,
    };

    data->Sprites[SpriteID::SPR_BLOCK_UP_WIDE_60] = {
        .id = SpriteID::SPR_BLOCK_UP_WIDE_60,
        .tx = 128 * 2,
        .ty = 64 + 128 * 1,
        .tw = 128,
        .th = 128,
        .originX = -2,
        .originY = 67,
    };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE] = {
        .id = SpriteID::SPR_BLOCK_WIDE,
        .tx = 128 * 3,
        .ty = 64 + 128 * 1,
        .tw = 128,
        .th = 128,
        .originX = 16,
        .originY = 76,
    };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_WIDE_30] = {
        .id = SpriteID::SPR_BLOCK_WIDE_WIDE_30,
        .tx = 128 * 4,
        .ty = 64 + 128 * 1,
        .tw = 128,
        .th = 128,
        .originX = 8,
        .originY = 80,
    };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_WIDE_60] = {
        .id = SpriteID::SPR_BLOCK_WIDE_WIDE_60,
        .tx = 128 * 0,
        .ty = 64 + 128 * 2,
        .tw = 128,
        .th = 128,
        .originX = -8,
        .originY = 87,
    };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_UP_30] = {
        .id = SpriteID::SPR_BLOCK_LONG_UP_30,
        .tx = 128 * 1,
        .ty = 64 + 128 * 2,
        .tw = 128,
        .th = 128,
        .originX = 68,
        .originY = 66,
    };

    data->Sprites[SpriteID::SPR_BLOCK_LONG_UP_60] = {
        .id = SpriteID::SPR_BLOCK_LONG_UP_60,
        .tx = 128 * 2,
        .ty = 64 + 128 * 2,
        .tw = 128,
        .th = 128,
        .originX = 45,
        .originY = 78,
    };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_UP_30] = {
        .id = SpriteID::SPR_BLOCK_WIDE_UP_30,
        .tx = 128 * 3,
        .ty = 64 + 128 * 2,
        .tw = 128,
        .th = 128,
        .originX = 67,
        .originY = 102,
    };

    data->Sprites[SpriteID::SPR_BLOCK_WIDE_UP_60] = {
        .id = SpriteID::SPR_BLOCK_WIDE_UP_60,
        .tx = 128 * 4,
        .ty = 64 + 128 * 2,
        .tw = 128,
        .th = 128,
        .originX = 44,
        .originY = 90,
    };

    data->Sprites[SpriteID::SPR_TITLE] = {
        .id = SpriteID::SPR_TITLE,
        .tx = 0,
        .ty = 448,
        .tw = 390,
        .th = 235,
        .originX = 0,
        .originY = 0,
    };

// levels
#include "./../assets-build/levels/gen_level_data.cpp.part"
}

void LevelData::print(FILE* file) const
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

            fprintf(f, "        {.x = %d, .y = %d, .type = LevelSwitchType::%s, .floorX = %d, .floorY = %d}", sw.x,
                    sw.y, swtype.c_str(), sw.floorX, sw.floorY);

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

LevelData generateRandomLevel(int pathLength)
{
    LevelData level{};
    level.cols = MAX_GRID_SIZE;
    level.rows = MAX_GRID_SIZE;

    std::string grid(level.cols * level.rows, '0');
    std::vector<vec2> path;
    std::random_device rd;
    std::mt19937 rng(rd());

    BlockSim block;
    block.x = rand() % level.cols;
    block.y = rand() % level.rows;
    block.state = BlockState::UP;

    auto [startA, startB] = block.getPositions();
    if (startA.x < 0 || startA.x >= level.cols || startA.y < 0 || startA.y >= level.rows)
        return level;

    path.push_back(startA);
    grid[startA.y * level.cols + startA.x] = '2';

    for (int i = 1; i < pathLength; ++i)
    {
        BlockSim next = block;
        std::vector<vec2> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        std::shuffle(directions.begin(), directions.end(), rng);

        bool moved = false;
        for (auto dir : directions)
        {
            next = block;
            next.move(dir);
            auto [a, b] = next.getPositions();
            if (a.x >= 0 && a.x < level.cols && a.y >= 0 && a.y < level.rows && b.x >= 0 && b.x < level.cols &&
                b.y >= 0 && b.y < level.rows && grid[a.y * level.cols + a.x] == '0' &&
                grid[b.y * level.cols + b.x] == '0')
            {
                block = next;
                grid[a.y * level.cols + a.x] = '1';
                grid[b.y * level.cols + b.x] = '1';
                path.push_back(a);
                moved = true;
                break;
            }
        }

        if (!moved)
            break;
    }

    auto [endA, endB] = block.getPositions();
    if (block.state == BlockState::UP)
        grid[endA.y * level.cols + endA.x] = '3';

    strncpy(level.data, grid.c_str(), level.cols * level.rows);
    level.data[level.cols * level.rows] = '\0';

    if (path.size() >= 5 && level.switchCount < MAX_SWITCH_COUNT)
    {
        level.switchCount = 1;
        level.switches[0].x = path[2].x;
        level.switches[0].y = path[2].y;
        level.switches[0].type = LevelSwitchType::SINGLE;
        level.switches[0].floorX = path[3].x;
        level.switches[0].floorY = path[3].y;
    }

    trimLevel(level);

    return level;
}

void trimLevel(LevelData& level)
{
    int minX = level.cols, maxX = -1;
    int minY = level.rows, maxY = -1;

    for (int y = 0; y < level.rows; ++y)
    {
        for (int x = 0; x < level.cols; ++x)
        {
            if (level.data[y * level.cols + x] != '0')
            {
                minX = std::min(minX, x);
                maxX = std::max(maxX, x);
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
            }
        }
    }

    if (minX > maxX || minY > maxY)
    {
        level.cols = level.rows = 0;
        level.data[0] = '\0';
        return;
    }

    int newCols = maxX - minX + 1;
    int newRows = maxY - minY + 1;
    char newData[MAX_GRID_SIZE * MAX_GRID_SIZE + 1];

    for (int y = 0; y < newRows; ++y)
    {
        for (int x = 0; x < newCols; ++x)
        {
            newData[y * newCols + x] = level.data[(y + minY) * level.cols + (x + minX)];
        }
    }

    newData[newCols * newRows] = '\0';
    memcpy(level.data, newData, sizeof(newData));
    level.cols = newCols;
    level.rows = newRows;

    for (int i = 0; i < level.switchCount; ++i)
    {
        level.switches[i].x -= minX;
        level.switches[i].y -= minY;
        level.switches[i].floorX -= minX;
        level.switches[i].floorY -= minY;
    }
}
