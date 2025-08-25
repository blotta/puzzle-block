#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
#include <initializer_list>

void IsoToWorld(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldToIso(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldtoGrid(int worldX, int worldY, int cellSize, int cols, int rows, int* x, int* y);

int cycleIndex(int currIdx, int length, int amount);

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

#endif