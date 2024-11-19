#include <stdio.h>
#include <SDL2/SDL.h>

bool running;

SDL_Renderer *renderer;
SDL_Window *window;

int frameCount, timerFPS, lastFrame, fps;

void update() {}
void input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        }
    }
}
void draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 100, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect = {
        20, 20, 50, 50};
    SDL_SetRenderDrawColor(renderer, 20, 80, 200, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

}

int main(int argc, char **args)
{
    running = true;
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowTitle(window, "Game");

    while (running)
    {
        lastFrame = SDL_GetTicks();
        update();
        input();
        draw();
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}