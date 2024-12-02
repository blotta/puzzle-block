#include "util.hpp"

void toISO(int x, int y, int tile_width, int tile_height, int* sx, int* sy)
{
    *sx = ((x * tile_width / 2) - (y * tile_width / 2));
    *sy = ((y * tile_height / 2) + (x * tile_height / 2));
}