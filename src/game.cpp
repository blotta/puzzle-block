#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.hpp"
#include "scene.hpp"
#include "scene_splash.hpp"
#include "scene_boot.hpp"

Game::Game() {}

Game::~Game()
{
    if (mInitialized)
    {
        SDL_Log("Deinitializing game\n");
        unloadAssets();
        TTF_Quit();
        IMG_Quit();
        SDL_DestroyRenderer(mRenderer);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }
}

bool Game::init()
{
    bool success = true;
    mRunning = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Failed to initialize SDL\n");
        success = false;
    }

    mWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->ScreenWidth, this->ScreenHeight, SDL_WINDOW_SHOWN);
    if (!mWindow)
    {
        SDL_Log("Failed to initialize SDL Window\n");
        success = false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
    {
        SDL_Log("Failed to initialize SDL Renderer\n");
        success = false;
    }
    SDL_SetWindowTitle(mWindow, "Game");

    if (!IMG_Init(IMG_INIT_PNG))
    {
        SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
        success = false;
    }

    if (-1 == TTF_Init())
    {
        SDL_Log("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        success = false;
    }

    mInitialized = success;
    return success;
}

bool Game::isRunning()
{
    return mRunning;
}

bool Game::loadAssets()
{
    bool success = true;

    mFont = TTF_OpenFont("assets/fonts/Cabin-Regular.ttf", 28);
    if (!mFont)
    {
        SDL_Log("Failed to laod font: %s\n", TTF_GetError());
        success = false;
    }

    SDL_Texture* tex = NULL;
    tex = IMG_LoadTexture(mRenderer, "assets/images/splash.png");
    if (!tex)
    {
        SDL_Log("Couldn't load splash.png: %s\n", SDL_GetError());
        success = false;
    }
    mTextures["splash"] = tex;

    return success;
}

void Game::unloadAssets()
{
    TTF_CloseFont(mFont);
    for (auto it = mTextures.begin(); it != mTextures.end(); it++)
    {
        SDL_DestroyTexture(it->second);
    }
}

void Game::loadScene(Scenes scene)
{
    if (scene == Scenes::BOOT)
    {
        SDL_Log("Loading scene\n");
        mCurrentScene = std::make_shared<BootScene>(this);
    }

    if (scene == Scenes::SPLASH)
    {
        SDL_Log("Loading scene\n");
        mCurrentScene = std::make_shared<SplashScene>(this);
    }
}


void Game::tick()
{
    input();
    update();
    draw();
    SDL_Delay(1000/TargetFPS);
}

SDL_Renderer *Game::getRenderer()
{
    return mRenderer;
}

SDL_Texture *Game::getTexture(const char *textureName)
{
    return mTextures.at(textureName);
}

TTF_Font *Game::getFont()
{
    return mFont;
}

void Game::input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            mRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_RIGHT:
                    loadScene(Scenes::SPLASH);
                    break;
                case SDLK_LEFT:
                    loadScene(Scenes::BOOT);
                    break;
            }
        }
    }

    mCurrentScene->input();
}

void Game::update()
{
    mCurrentScene->update();
}

void Game::draw()
{
    mCurrentScene->draw();
}

void Game::run()
{
    SDL_Log("Starting program\n");
    mRunning = true;

    if (!init())
    {
        SDL_Log("Failed to initialize\n");
    }
    else
    {
        if (!loadAssets())
        {
            SDL_Log("Failed to load media\n");
        }
        else
        {
            loadScene(Scenes::BOOT);
            while (mRunning)
            {
                tick();
            }
        }
    }
}