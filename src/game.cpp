#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.hpp"
#include "scene.hpp"
#include "scene_splash.hpp"
#include "scene_boot.hpp"
#include "scene_level.hpp"
#include "scene_isolevel.hpp"

Game::Game()
    : mAsset(nullptr)
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

    SDL_SetWindowTitle(mWindow, "Game");

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
    {
        SDL_Log("Failed to initialize SDL Renderer\n");
        success = false;
    }
    mAsset.setRenderer(mRenderer);

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

    mUpdateTimer.setDuration(1.0 / TargetFPS);
    mUpdateTimer.reset();

    mFPSTimer.setDuration(1.0);
    mFPSTimer.reset();

    if (!success)
        SDL_Log("Failed to initialize\n");
}

Game::~Game()
{
    SDL_Log("Deinitializing game\n");

    // make sure to deinit assets before deiniting systems
    unloadAssets();

    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
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
        SDL_Log("Failed to load font: %s\n", TTF_GetError());
        success = false;
    }

    // preload textures
    mAsset.getTexture("assets/images/splash.png");
    mAsset.getTexture("assets/images/spritesheet.png");

    loadLevels();

    return success;
}

void Game::unloadAssets()
{
    mAsset.unloadAssets();
    TTF_CloseFont(mFont);
}

void Game::loadLevels()
{
    for (auto &lvl : DEFAULT_LEVELS)
    {
        mLevels.emplace_back(lvl);
    }
}

void Game::loadScene(Scenes scene)
{
    switch (scene)
    {
    case Scenes::SPLASH:
        SDL_Log("Loading Splash scene\n");
        mCurrentScene = std::make_shared<SplashScene>(this);
        break;
    case Scenes::LEVEL:
        SDL_Log("Loading Level scene\n");
        mCurrentScene = std::make_shared<LevelScene>(this);
        break;
    case Scenes::ISOLEVEL:
        SDL_Log("Loading ISO Level scene\n");
        mCurrentScene = std::make_shared<IsoLevelScene>(this);
        break;
    default:
        SDL_Log("Loading default scene\n");
        mCurrentScene = std::make_shared<BootScene>(this);
        break;
    }
}

void Game::tick()
{
    mUpdateTimer.waitUntilDone();
    float dt = mUpdateTimer.elapsed();
    mUpdateTimer.reset();

    _input(dt);
    update(dt);
    draw();

    if (mFPSTimer.isDone())
    {
        mFPSTimer.reset();
        // SDL_Log("FPS: %d ; Avg DT: %.5f\n", mFpsCounter, mAvgDt);
        mFpsCounter = 0;
    }
    mFpsCounter += 1;
}

SDL_Renderer *Game::getRenderer()
{
    return mRenderer;
}

const Texture* Game::getTTexture(const std::string &texture)
{
    return mAsset.getTexture(texture);
}

TTF_Font *Game::getFont()
{
    return mFont;
}

const std::string Game::getState(const std::string &name)
{
    return mState.at(name);
}

const std::string Game::getOrCreateState(const std::string &name, const std::string &value)
{
    mState.try_emplace(name, value);

    return mState.at(name);
}

void Game::setState(const std::string &name, const std::string &value)
{
    mState[name] = value;
}

const LevelData &Game::getLevel(int idx)
{
    return mLevels[idx];
}

void Game::_input(float dt)
{
    input.update(dt);
    if (input.quit_requested())
        mRunning = false;

    mCurrentScene->input();
}

void Game::update(float dt)
{
    mCurrentScene->update(dt);
}

void Game::draw()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    mCurrentScene->draw();

    SDL_RenderPresent(mRenderer);
}

void Game::run()
{
    SDL_Log("Starting program\n");
    mRunning = true;

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