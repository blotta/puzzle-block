#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.hpp"
#include "scene.hpp"
#include "scene_splash.hpp"
#include "scene_boot.hpp"
#include "scene_leveledit.hpp"
#include "scene_isolevel.hpp"

Game &Game::get()
{
    static Game instance;
    return instance;
}

Game::Game()
{
    bool success = true;
    mRunning = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Failed to initialize SDL\n");
        success = false;
    }

    mWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->mScreenWidth, this->mScreenHeight, SDL_WINDOW_SHOWN);
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
    Asset::SetRenderer(mRenderer);

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

    mUpdateTimer.setDuration(1.0 / mTargetFPS);
    mUpdateTimer.reset();

    mFPSTimer.setDuration(1.0);
    mFPSTimer.reset();

    load_game_data(&mData);

    if (!success)
        SDL_Log("Failed to initialize\n");
}

Game::~Game()
{
    SDL_Log("Deinitializing game\n");

    // make sure to deinit assets before deiniting systems
    Asset::UnloadAssets();

    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

int Game::ScreenWidth()
{
    return Game::get().mScreenWidth;
}

int Game::ScreenHeight()
{
    return Game::get().mScreenHeight;
}

int Game::TargetFPS()
{
    return Game::get().mTargetFPS;
}


void Game::LoadScene(Scenes sceneName)
{
    Game::get().loadScene(sceneName);
}



SDL_Renderer *Game::GetRenderer()
{
    return Game::get().mRenderer;
}

const std::string Game::GetState(const std::string &name)
{
    return Game::get().mState.at(name);
}

const std::string Game::GetOrCreateState(const std::string &name, const std::string &value)
{
    auto& g = Game::get();
    g.mState.try_emplace(name, value);
    return g.mState.at(name);
}

void Game::SetState(const std::string &name, const std::string &value)
{
    Game::get().mState[name] = value;
}

const LevelData &Game::GetLevelData(int idx)
{
    return Game::get().mLevels[idx];
}

void Game::SaveLevelData(const LevelData &ld, int idx)
{
    Game::get().mLevels[idx] = ld;
}

const Sprite &Game::GetSprite(SpriteID id)
{
    return Game::get().mData.Sprites[id];
}

void Game::DrawSprite(int x, int y, SpriteID sprId)
{
    auto& g = Game::get();
    const Sprite* spr = &g.mData.Sprites[sprId];
    SDL_Rect src = {
        spr->tx,
        spr->ty,
        spr->tw,
        spr->th
    };
    SDL_Rect dest = {
        x - spr->originX,
        y - spr->originY,
        spr->tw,
        spr->th
    };

    SDL_RenderCopy(g.mRenderer, g.pActiveTexture->get(), &src, &dest);
}

void Game::Run()
{
    Game::get().run();
}


//////////////////////
// Instance Methods //
//////////////////////

void Game::run()
{
    SDL_Log("Starting program\n");
    mRunning = true;

    loadAssets();
    loadScene(Scenes::BOOT);
    while (mRunning)
    {
        tick();
    }
}

void Game::tick()
{
    mUpdateTimer.waitUntilDone();
    float dt = mUpdateTimer.elapsed();
    mUpdateTimer.reset();

    input(dt);
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

void Game::loadAssets()
{
    Asset::LoadFont("assets/fonts/Cabin-Regular.ttf", 28);

    // preload spritesheet
    pActiveTexture = Asset::GetTexture("assets/images/spritesheet.png");

    loadLevels();
}

void Game::loadLevels()
{
    for (auto &lvl : mData.DefaultLevels)
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
        mCurrentScene = std::make_shared<SplashScene>();
        break;
    case Scenes::LEVEL_EDIT:
        SDL_Log("Loading Level scene\n");
        mCurrentScene = std::make_shared<LevelEditScene>();
        break;
    case Scenes::ISOLEVEL:
        SDL_Log("Loading ISO Level scene\n");
        mCurrentScene = std::make_shared<IsoLevelScene>();
        break;
    default:
        SDL_Log("Loading default scene\n");
        mCurrentScene = std::make_shared<BootScene>();
        break;
    }
}

void Game::input(float dt)
{
    Input::Update(dt);
    if (Input::QuitRequested())
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




