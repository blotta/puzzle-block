#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "game.hpp"
#include "input_manager.hpp"
#include "asset_manager.hpp"
#include "log.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Game& Game::get()
{
    static Game instance;
    return instance;
}

Game::Game()
{
}

Game::~Game()
{
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

const GameSettings& Game::Settings()
{
    return Game::get().settings;
}

void Game::LoadScene(Scenes sceneName)
{
    auto& g = Game::get();
    g.mSceneManager.requestSwitch(sceneName);
}

void Game::PushScene(Scenes sceneName)
{
    auto& g = Game::get();
    g.mSceneManager.pushScene(sceneName);
}

void Game::PopScene()
{
    auto& g = Game::get();
    g.mSceneManager.popScene();
}

SDL_Renderer* Game::GetRenderer()
{
    return Game::get().mRenderer;
}

const std::string Game::GetState(const std::string& name)
{
    return Game::get().mState.at(name);
}

const std::string Game::GetOrCreateState(const std::string& name, const std::string& value)
{
    auto& g = Game::get();
    g.mState.try_emplace(name, value);
    return g.mState.at(name);
}

void Game::SetState(const std::string& name, const std::string& value)
{
    Game::get().mState[name] = value;
}

const int Game::GetLevelsSize()
{
    return Game::get().mLevels.size();
}

const std::vector<LevelData>& Game::GetLevels()
{
    return Game::get().mLevels;
}

const LevelData& Game::GetLevelData(int idx)
{
    return Game::get().mLevels[idx];
}

void Game::SaveLevelData(const LevelData& ld, int idx)
{
    Game::get().mLevels[idx] = ld;
}

int Game::AddLevelData(const LevelData& ld)
{
    Game::get().mLevels.emplace_back(ld);
    return Game::get().mLevels.size() - 1;
}

const Sprite& Game::GetSprite(SpriteID id)
{
    return Game::get().mData.Sprites[id];
}

void Game::DrawSprite(int x, int y, SpriteID sprId)
{
    auto& g = Game::get();
    const Sprite* spr = &g.mData.Sprites[sprId];
    SDL_Rect src = {spr->tx, spr->ty, spr->tw, spr->th};
    SDL_Rect dest = {x - spr->originX, y - spr->originY, spr->tw, spr->th};

    g.pActiveTexture->drawTile(&src, &dest);
}

void Game::PlaySound(const std::string& path)
{
    auto sound = Asset::GetSound(path);
    Mix_PlayChannel(-1, sound, 0);
}

void Game::SetSoundVolume(int vol)
{
    if (vol > 10)
        vol = 10;
    if (vol < 0)
        vol = 0;

    int mixVol = MIX_MAX_VOLUME * (vol/10.f);
    Mix_Volume(-1, mixVol);
    Game::get().settings.sfx_vol = vol;
}

void Game::PlayMusic(const std::string& path)
{
    auto sound = Asset::GetMusic(path);
    if (0 == Mix_PlayingMusic())
        Mix_PlayMusic(sound, -1);
}

void Game::SetMusicVolume(int vol)
{
    if (vol > 10)
        vol = 10;
    if (vol < 0)
        vol = 0;

    int mixVol = MIX_MAX_VOLUME * (vol/10.f);
    Mix_VolumeMusic(mixVol);
    Game::get().settings.music_vol = vol;
}

void Game::SetFont(const std::string& path, int ptsize)
{
    auto& g = Game::get();
    g.mActiveFont = Asset::GetFont(path, ptsize);
}

void Game::SetFontSize(int ptsize)
{
    auto& g = Game::get();
    g.mActiveFont = Asset::GetFont(g.mActiveFont->fontPath, ptsize);
}

void Game::Text(int x, int y, const std::string& text, const FontDrawOptions& options)
{
    auto& g = Game::get();
    g.mActiveFont->drawText(x, y, text, options);
}

int Game::TextWidth(const std::string& text, const FontDrawOptions& options)
{
    auto& g = Game::get();
    return g.mActiveFont->measureText(text, options).width;
}

void Game::Run()
{
    Game::get().run();
}

void Game::Exit()
{
    Game::get().mRunning = false;
}


//////////////////////
// Instance Methods //
//////////////////////

void Game::init()
{
    bool success = true;
    mRunning = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Log::error("Failed to initialize SDL\n");
        success = false;
    }

    // #ifdef DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    // #endif

    mWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->mScreenWidth,
                               this->mScreenHeight, SDL_WINDOW_SHOWN);
    if (!mWindow)
    {
        Log::error("Failed to initialize SDL Window\n");
        success = false;
    }

    SDL_SetWindowTitle(mWindow, "Game");

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
    {
        Log::error("Failed to initialize SDL Renderer\n");
        success = false;
    }
    Asset::SetRenderer(mRenderer);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        Log::error("Failed to initialize SDL_image: %s\n", IMG_GetError());
        success = false;
    }

    if (-1 == TTF_Init())
    {
        Log::error("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        success = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        Log::error("Failed to initialize SDL_mixer: %s\n", Mix_GetError());
        success = false;
    }
    SetSoundVolume(settings.sfx_vol);
    SetMusicVolume(settings.music_vol);

    mUpdateTimer.setDuration(1.0 / mTargetFPS);
    mUpdateTimer.reset();

    mFPSTimer.setDuration(1.0);
    mFPSTimer.reset();

    loadAssets();

    mSceneManager.init(mStartScene);

    if (!success)
        Log::error("Failed to initialize\n");
}

void Game::dispose()
{
    Log::info("Deinitializing game\n");

    // unload scene
    mSceneManager.dispose();

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

void Game::run()
{
    Log::info("Starting program\n");
    mRunning = true;

    init();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop([]() { Game::get().tick(); }, 0, 1);
#else
    while (mRunning)
    {
        tick();
    }
#endif

    dispose();
}

void Game::tick()
{
    mUpdateTimer.waitUntilDone();
    float dt = mUpdateTimer.elapsed();
    mUpdateTimer.reset();

    Input::Update(dt);

    if (Input::QuitRequested())
    {
        Game::Exit();
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_F12))
    {
        mSceneManager.requestSwitch(mStartScene);
    }

    // update
    mSceneManager.update(dt);

    // draw
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);
    mSceneManager.draw();
    SDL_RenderPresent(mRenderer);

    if (mFPSTimer.isDone())
    {
        mFPSTimer.reset();
        // Log::info("FPS: %d ; Avg DT: %.5f\n", mFpsCounter, mAvgDt);
        mFpsCounter = 0;
    }
    mFpsCounter += 1;
}

void Game::loadAssets()
{
    Asset::LoadFont("assets/fonts/Cabin-Regular.ttf", 16);
    Asset::LoadFont("assets/fonts/Cabin-Regular.ttf", 20);
    Game::SetFont("assets/fonts/Cabin-Regular.ttf", 32);

    // preload spritesheet
    pActiveTexture = Asset::GetTexture("assets/images/spritesheet.png");

    Asset::LoadSound("assets/sfx/block_move.ogg");
    Asset::LoadSound("assets/sfx/switch.ogg");
    Asset::LoadSound("assets/sfx/arrive.ogg");
    Asset::LoadSound("assets/sfx/swosh-05.ogg");
    Asset::LoadSound("assets/sfx/swosh-15.ogg");
    Asset::LoadSound("assets/sfx/swosh-18.ogg");
    Asset::LoadSound("assets/sfx/swosh-37-mod.ogg");
    Asset::LoadSound("assets/sfx/rumble.ogg");
    Asset::LoadSound("assets/sfx/snd_gunshot1.ogg");
    Asset::LoadSound("assets/sfx/uff.ogg");
    Asset::LoadSound("assets/sfx/ui_move.ogg");

    Asset::LoadMusic("assets/sfx/music_ambient_01.ogg");

    loadLevels();
}

void Game::loadLevels()
{
    load_game_data(&mData);
    for (auto& lvl : mData.DefaultLevels)
    {
        mLevels.emplace_back(lvl);
    }
}
