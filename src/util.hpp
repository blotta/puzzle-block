#ifndef UTIL_HPP
#define UTIL_HPP

struct vec2
{
    int x = 0;
    int y = 0;
    vec2()
    {
        x = 0;
        y = 0;
    }
    vec2(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

struct vec2f
{
    float x = 0;
    float y = 0;
    vec2f()
    {
        x = 0;
        y = 0;
    }
    vec2f(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

void IsoToWorld(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldToIso(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldtoGrid(int worldX, int worldY, int cellSize, int cols, int rows, int* x, int* y);

float linear_interpolate(float v0, float v1, float t);

int cycleIndex(int currIdx, int length, int amount);

#endif