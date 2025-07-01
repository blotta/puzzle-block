#ifndef UTIL_HPP
#define UTIL_HPP

#include "data.hpp"

struct vec2
{
    int x, y;
    vec2() : x(0), y(0)
    {
    }
    vec2(int x, int y) : x(x), y(y)
    {
    }

    vec2 operator+(const vec2& other) const
    {
        return vec2(this->x + other.x, this->y + other.y);
    }
    vec2 operator-(const vec2& other) const
    {
        return vec2(this->x - other.x, this->y - other.y);
    }
    vec2 operator*(int scalar) const
    {
        return vec2(this->x * scalar, this->y * scalar);
    }
    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

struct vec2f
{
    float x, y;
    vec2f() : x(0), y(0)
    {
    }
    vec2f(float x, float y) : x(x), y(y)
    {
    }

    vec2f operator+(const vec2f& other) const
    {
        return vec2f(this->x + other.x, this->y + other.y);
    }
    vec2f operator-(const vec2f& other) const
    {
        return vec2f(this->x - other.x, this->y - other.y);
    }
    vec2f operator*(float scalar) const
    {
        return vec2f(this->x * scalar, this->y * scalar);
    }

    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

namespace easings
{
// linear
float lerp(float a, float b, float t);
int lerp(int a, int b, float t);
vec2f lerp(const vec2f& a, const vec2f& b, float t);
vec2 lerp(const vec2& a, const vec2& b, float t);
SpriteID lerp(const SpriteID& a, const SpriteID& b, float t);

// ease in
float easeIn(float a, float b, float t);
int easeIn(int a, int b, float t);
vec2f easeIn(const vec2f& a, const vec2f& b, float t);
vec2 easeIn(const vec2& a, const vec2& b, float t);
SpriteID easeIn(const SpriteID& a, const SpriteID& b, float t);

// ease out
float easeOut(float a, float b, float t);
int easeOut(int a, int b, float t);
vec2f easeOut(const vec2f& a, const vec2f& b,  float t);
vec2 easeOut(const vec2& a, const vec2& b, float t);
SpriteID easeOut(const SpriteID& a, const SpriteID& b, float t);

// ease out bounce
float easeOutBounce(float a, float b, float t);
int easeOutBounce(int a, int b, float t);
vec2f easeOutBounce(const vec2f& a, const vec2f& b, float t);
vec2 easeOutBounce(const vec2& a, const vec2& b, float t);
SpriteID easeOutBounce(const SpriteID& a, const SpriteID& b, float t);
} // namespace easings

void IsoToWorld(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldToIso(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldtoGrid(int worldX, int worldY, int cellSize, int cols, int rows, int* x, int* y);

float linear_interpolate(float v0, float v1, float t);

int cycleIndex(int currIdx, int length, int amount);

#endif