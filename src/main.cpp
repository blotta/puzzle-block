#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

bool running, success = false;

SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
SDL_Texture *splashTexture = NULL;

int frameCount, timerFPS, lastFrame, fps;

bool init();
bool loadMedia();
void close();

bool init()
{
    running = true;
    success = true;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowTitle(window, "Game");

    IMG_Init(IMG_INIT_PNG);

    return success;
}

bool loadMedia()
{
    success = true;

    splashTexture = IMG_LoadTexture(renderer, "assets/images/splash.png");
    if (!splashTexture)
    {
        SDL_Log("Couldn't load splash.png: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

void close()
{
    SDL_DestroyTexture(splashTexture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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

    SDL_Rect splash = {
        300, 300, 400, 200};
    SDL_RenderCopy(renderer, splashTexture, NULL, &splash);

    SDL_RenderPresent(renderer);
}

void loop()
{
    // lastFrame = SDL_GetTicks();
    update();
    input();
    draw();
    SDL_Delay(16);
}

int main(int argc, char **args)
{

    SDL_Log("Starting program\n");

    if (!init())
    {
        SDL_Log("Failed to initialize\n");
    }
    else
    {
        if (!loadMedia())
        {
            SDL_Log("Failed to load media\n");
        }
        else
        {
            while (running)
            {
                loop();
            }
        }
    }

    close();
    return 0;
}