#include "util.hpp"
#include "data.hpp"
#include <math.h>
#include <random>

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

// easeOutBounce functions
float easeOutBounce(float a, float b, float t)
{
    if (t < 1.0f / 2.75f)
    {
        return lerp(a, b, 7.5625f * t * t);
    }
    else if (t < 2.0f / 2.75f)
    {
        t -= 1.5f / 2.75f;
        return lerp(a, b, 7.5625f * t * t + 0.75f);
    }
    else if (t < 2.5f / 2.75f)
    {
        t -= 2.25f / 2.75f;
        return lerp(a, b, 7.5625f * t * t + 0.9375f);
    }
    else
    {
        t -= 2.625f / 2.75f;
        return lerp(a, b, 7.5625f * t * t + 0.984375f);
    }
}

int easeOutBounce(int a, int b, float t)
{
    return static_cast<int>(easeOutBounce(static_cast<float>(a), static_cast<float>(b), t));
}

vec2f easeOutBounce(const vec2f& a, const vec2f& b, float t)
{
    return vec2f(easeOutBounce(a.x, b.x, t), easeOutBounce(a.y, b.y, t));
}

vec2 easeOutBounce(const vec2& a, const vec2& b, float t)
{
    return vec2(static_cast<int>(easeOutBounce(static_cast<float>(a.x), static_cast<float>(b.x), t)),
                static_cast<int>(easeOutBounce(static_cast<float>(a.y), static_cast<float>(b.y), t)));
}

SpriteID easeOutBounce(const SpriteID& a, const SpriteID& b, float t)
{
    return a;
}

// easeOutBack functions
float easeOutBack(float a, float b, float t)
{
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    float tt = t - 1.0f;
    float easedT = 1.0f + c3 * (tt * tt * tt) + c1 * (tt * tt);
    return a + (b - a) * easedT;
}

int easeOutBack(int a, int b, float t)
{
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    float tt = t - 1.0f;
    float easedT = 1.0f + c3 * (tt * tt * tt) + c1 * (tt * tt);
    return static_cast<int>(a + (b - a) * easedT);
}

vec2f easeOutBack(const vec2f& a, const vec2f& b, float t)
{
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    float tt = t - 1.0f;
    float easedT = 1.0f + c3 * (tt * tt * tt) + c1 * (tt * tt);
    return a + (b - a) * easedT;
}

vec2 easeOutBack(const vec2& a, const vec2& b, float t)
{
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    float tt = t - 1.0f;
    float easedT = 1.0f + c3 * (tt * tt * tt) + c1 * (tt * tt);
    return vec2(
        static_cast<int>(a.x + (b.x - a.x) * easedT),
        static_cast<int>(a.y + (b.y - a.y) * easedT)
    );
}

SpriteID easeOutBack(const SpriteID& a, const SpriteID& b, float t)
{
    // No interpolation logic for SpriteID — same as easeIn
    return a;
}

// snap functions
float snap(float a, float b, float t)
{
    return (t >= 1.0f) ? b : a;
}

int snap(int a, int b, float t)
{
    return (t >= 1.0f) ? b : a;
}

vec2f snap(const vec2f& a, const vec2f& b, float t)
{
    return (t >= 1.0f) ? b : a;
}

vec2 snap(const vec2& a, const vec2& b, float t)
{
    return (t >= 1.0f) ? b : a;
}

SpriteID snap(const SpriteID& a, const SpriteID& b, float t)
{
    return (t >= 1.0f) ? b : a;
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

float random01()
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng);
}

float randomNeg1to1()
{
    return random01() * 2.0f - 1.0f;
}