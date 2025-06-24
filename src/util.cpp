#include "util.hpp"
#include <math.h>

void IsoToWorld(int isoX, int isoY, int tile_width, int tile_height, int* worldX, int* worldY)
{
    *worldX = ((isoX * tile_width / 2) - (isoY * tile_width / 2));
    *worldY = ((isoY * tile_height / 2) + (isoX * tile_height / 2));
}

void WorldToIso(int worldX, int worldY, int tile_width, int tile_height, int* isoX, int* isoY)
{
    float tileWidthHalfF = (float)tile_width / 2.0f;
    float tileHeightHalfF = (float)tile_height / 2.0f;
    float worldXf = (float)(worldX - tile_width / 2); // drawing origin for each tile in a grid is top-left corner, but
                                                      // calculation is based on the top corner of the tile
    float worldYf = (float)(worldY);

    *isoX = ((worldXf / tileWidthHalfF) + (worldYf / tileHeightHalfF)) / 2;
    *isoY = ((worldYf / tileHeightHalfF) - (worldXf / tileWidthHalfF)) / 2;
}

void WorldtoGrid(int worldX, int worldY, int cellSize, int cols, int rows, int* x, int* y)
{
    *x = floor(worldX / cellSize);
    if (worldX < 0)
        *x -= 1;
    *y = floor(worldY / cellSize);
    if (worldY < 0)
        *y -= 1;
}

float linear_interpolate(float v0, float v1, float t)
{
    return (1.f - t) * v0 + t * v1;
}

int cycleIndex(int currIdx, int length, int amount)
{
    if (amount == 0)
        return currIdx;

    int next = currIdx + amount;

    while (next >= length || next < 0)
    {
        if (next >= length)
            next = next - length;
        else if (next < 0)
            next = next + length;
    }

    return next;
}