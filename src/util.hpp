#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
#include <initializer_list>
#include <SDL2/SDL.h>

void IsoToWorld(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldToIso(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldtoGrid(int worldX, int worldY, int cellSize, int cols, int rows, int* x, int* y);

int cycleIndex(int currIdx, int length, int amount);

int clamp(int val, int min, int max);
int min(int a, int b);
int max(int a, int b);

float clamp(float val, float min, float max);
float min(float a, float b);
float max(float a, float b);

float random01();
float randomNeg1to1();

template <typename E>
E randomFrom(std::initializer_list<E> values) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, values.size() - 1);

    auto it = values.begin();
    std::advance(it, dist(gen));
    return *it;
}

typedef uint8_t u8;

struct Color
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    Color();
    Color(const SDL_Color& sdlColor);
    Color(int r, int g, int b, int a);
    Color(int rgb);
    Color(int rgb, int a);

    Color operator+(int val) const;
    SDL_Color toSDL() const;
};

#endif