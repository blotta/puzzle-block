#ifndef UTIL_HPP
#define UTIL_HPP

void IsoToWorld(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldToIso(int x, int y, int tile_width, int tile_height, int* sx, int* sy);
void WorldtoGrid(int worldX, int worldY, int cellSize, int cols, int rows, int* x, int* y);

int cycleIndex(int currIdx, int length, int amount);

float random01();
float randomNeg1to1();
#endif