#include "util.hpp"
#include "data.hpp"
#include <math.h>

namespace easings
{

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

int lerp(int a, int b, float t)
{
    return static_cast<int>(a + (b - a) * t);
}

vec2f lerp(const vec2f& a, const vec2f& b, float t)
{
    return a + (b - a) * t;
}

vec2 lerp(const vec2& a, const vec2& b, float t)
{
    return vec2(static_cast<int>(a.x + (b.x - a.x) * t), static_cast<int>(a.y + (b.y - a.y) * t));
}

SpriteID lerp(const SpriteID& a, const SpriteID& b, float t)
{
    return a;
}

// easeIn functions
float easeIn(float a, float b, float t)
{
    return a + (b - a) * (t * t);
}
int easeIn(int a, int b, float t)
{
    return a + (b - a) * (t * t);
}
vec2f easeIn(const vec2f& a, const vec2f& b, float t)
{
    return a + (b - a) * (t * t);
}
vec2 easeIn(const vec2& a, const vec2& b, float t)
{
    return vec2(static_cast<int>(a.x + (b.x - a.x) * (t * t)), static_cast<int>(a.y + (b.y - a.y) * (t * t)));
}
SpriteID easeIn(const SpriteID& a, const SpriteID& b, float t)
{
    return a;
}

// easeOut functions
float easeOut(float a, float b, float t)
{
    return a + (b - a) * (1 - (1 - t) * (1 - t));
}

int easeOut(int a, int b, float t)
{
    return a + (b - a) * (1 - (1 - t) * (1 - t));
}

vec2f easeOut(const vec2f& a, const vec2f& b, float t)
{
    return a + (b - a) * (1 - (1 - t) * (1 - t));
}
vec2 easeOut(const vec2& a, const vec2& b, float t)
{
    return vec2(static_cast<int>(a.x + (b.x - a.x) * (1 - (1 - t) * (1 - t))),
                static_cast<int>(a.y + (b.y - a.y) * (1 - (1 - t) * (1 - t))));
}
SpriteID easeOut(const SpriteID& a, const SpriteID& b, float t)
{
    return a;
}

} // namespace easings
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