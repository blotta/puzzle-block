#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "game.hpp"

int main(int argc, char **args)
{
    Game game;
    game.run();

    return 0;
}