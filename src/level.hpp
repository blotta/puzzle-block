#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <array>
#include <vector>
#include <span>
#include <string_view>
#include <SDL2/SDL.h>

#include "game.hpp"
#include "util.hpp"

class Game;

enum class CellType
{
    EMPTY = 0,
    FLOOR = 1,
    START = 2,
    FINISH = 3
};
struct Level
{
    Level(Game *game);
    ~Level();

    Game *game;

    int rows = 6;
    int cols = 4;
    const Texture *pSpriteSheet;

    std::array<std::array<CellType, 10>, 10> mGrid = {{}};

    void clear();
    void set(int x, int y, CellType value);
    void load(const std::span<std::string_view> &ld);
    void load(const LevelData &ld);
    vec2 getStartPos();
    bool isValidPos(const vec2 &pos);
    bool hasFloorAt(const vec2 &pos);
    void draw(SDL_Renderer *rend, int x, int y, int cellSize);
    void drawISO(SDL_Renderer *rend, int x, int y, int cellSize);
};

#endif