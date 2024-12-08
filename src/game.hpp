#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <string>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "scene.hpp"
#include "input_manager.hpp"
#include "asset_manager.hpp"
#include "level.hpp"

#include "timer.hpp"

class Scene;
enum class Scenes;
struct LevelData;

class Game
{
public:
    Game(const Game&) = delete;

    static int ScreenWidth();
    static int ScreenHeight();
    static int TargetFPS();

    static void Run();
    static void LoadScene(Scenes sceneName);

    static SDL_Renderer* GetRenderer();

    static const std::string GetState(const std::string &name);
    static const std::string GetOrCreateState(const std::string &name, const std::string &value);
    static void SetState(const std::string &name, const std::string &value);

    static const int GetLevelsSize();
    static const LevelData& GetLevelData(int idx);
    static void SaveLevelData(const LevelData &ld, int idx);
    static int AddLevelData(const LevelData &ld);

    static const Sprite& GetSprite(SpriteID id);
    static void DrawSprite(int x, int y, SpriteID sprId);

private:
    Game();
    ~Game();
    static Game& get();

    void loadAssets();
    void loadLevels();
    void loadScene(Scenes sceneName);
    void run();
    // Runs update() and draw()
    void tick();
    void update(float dt);
    void draw();

    const int mScreenWidth = 1280;
    const int mScreenHeight = 720;
    const int mTargetFPS = 60;

    SDL_Window* mWindow = NULL;
    SDL_Renderer* mRenderer = NULL;

    Timer mUpdateTimer;
    Timer mFPSTimer;
    int mFpsCounter = 0;

    bool mRunning = false;

    const Texture* pActiveTexture;

    std::shared_ptr<Scene> mCurrentScene;
    std::map<std::string, std::string> mState;

    std::vector<LevelData> mLevels;

    // data
    GameData mData = {};
};

#endif