#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <array>
#include <vector>
#include <span>
#include <string_view>
#include <SDL2/SDL.h>

#include "game.hpp"
#include "util.hpp"


enum CellType
{
    EMPTY = 0,
    FLOOR = 1,
    START = 2,
    FINISH = 3
};
struct Level
{
    Level(Game* game);
    ~Level();

    Game* game;

    int rows = 6;
    int cols = 4;
    SDL_Texture* pSpriteSheet;

    std::array<std::array<CellType, 10>, 10> mGrid = {{}};

    void clear();
    void set(int x, int y, CellType value);
    void load(const std::span<std::string_view>& ld);
    vec2 getStartPos();
    bool isValidPos(const vec2& pos);
    bool hasFloorAt(const vec2& pos);
    void draw(SDL_Renderer* rend, int x, int y, int cellSize);
    void drawISO(SDL_Renderer* rend, int x, int y, int cellSize);
};

extern std::array<std::span<std::string_view>, 2> LEVELS;

#endif