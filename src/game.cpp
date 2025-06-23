#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "game.hpp"
#include "input_manager.hpp"
#include "scene_splash.hpp"
#include "scene_boot.hpp"
#include "scene_leveledit.hpp"
#include "scene_isolevel.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
        success = false;
    }

    if (-1 == TTF_Init())
    {
        SDL_Log("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        success = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("Failed to initialize SDL_mixer: %s\n", Mix_GetError());
        success = false;
    }

    Asset::LoadSound("assets/sfx/block_move.ogg");

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

    // unload scene
    loadScene(Scenes::NONE);

    this->mDynText.Destroy();

    // make sure to deinit assets before deiniting systems
    Asset::UnloadAssets();

    Mix_CloseAudio();
    Mix_Quit();
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
    Game::get().mNextScene = sceneName;
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

const int Game::GetLevelsSize()
{
    return Game::get().mLevels.size();
}

const LevelData &Game::GetLevelData(int idx)
{
    return Game::get().mLevels[idx];
}

void Game::SaveLevelData(const LevelData &ld, int idx)
{
    Game::get().mLevels[idx] = ld;
}

int Game::AddLevelData(const LevelData &ld)
{
    Game::get().mLevels.emplace_back(ld);
    return Game::get().mLevels.size() - 1;
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

void Game::DrawText(int x, int y, const std::string &txt)
{
    auto& g = Game::get();
    g.mDynText.Draw(x, y, txt);
}


void Game::PlaySound(const std::string& path)
{
    auto sound = Asset::GetSound(path);
    Mix_PlayChannel(-1, sound, 0);
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

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop([]() {
        Game::get().tick();
    }, 0, 1);
#else
    while (mRunning)
    {
        tick();
    }
#endif

    // TODO: move this somewhere else
    if (mCurrentScene != nullptr)
    {
        mCurrentScene->dispose();
    }
}

void Game::tick()
{
    mUpdateTimer.waitUntilDone();
    float dt = mUpdateTimer.elapsed();
    mUpdateTimer.reset();

    Input::Update(dt);

    if (Input::QuitRequested())
    {
        mRunning = false;
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_F11))
    {
        LoadScene(Scenes::BOOT);
    }

    // check scene changed
    if (mNextScene != Scenes::NONE)
        loadScene(mNextScene);

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
    Asset::LoadFont("assets/fonts/Cabin-Regular.ttf", 32);

    // preload spritesheet
    pActiveTexture = Asset::GetTexture("assets/images/spritesheet.png");

    mDynText.Init();

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
    if (scene == Scenes::NONE)
        return;
    
    if (mCurrentScene != nullptr)
        mCurrentScene->dispose();

    switch (scene)
    {
    case Scenes::SPLASH:
        mCurrentScene = std::make_shared<SplashScene>();
        break;
    case Scenes::LEVEL_EDIT:
        mCurrentScene = std::make_shared<LevelEditScene>();
        break;
    case Scenes::ISOLEVEL:
        mCurrentScene = std::make_shared<IsoLevelScene>();
        break;
    case Scenes::BOOT:
        mCurrentScene = std::make_shared<BootScene>();
        break;
    default:
        mCurrentScene = nullptr;
        break;
    }

    mCurrentScene->init();

    mNextScene = Scenes::NONE;
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




