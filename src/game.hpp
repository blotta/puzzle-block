#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <string>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "scene.hpp"
#include "asset.hpp"
#include "input_manager.hpp"
#include "level.hpp"

#include "timer.hpp"

class Scene;
enum class Scenes;
struct LevelData;

class Game
{
public:
    Game();
    ~Game();

    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    const int TargetFPS = 60;

    InputManager input;

    bool isRunning();
    void run();
    void loadScene(Scenes sceneName);

    SDL_Renderer *getRenderer();
    const Texture* getTexture(const std::string& texture);
    TTF_Font *getFont();
    const std::string getState(const std::string &name);
    const std::string getOrCreateState(const std::string &name, const std::string &value);
    void setState(const std::string &name, const std::string &value);
    const LevelData &getLevelData(int idx);
    void saveLevelData(const LevelData& ld, int idx);
    const Sprite& getSprite(SpriteID id);
    void drawSprite(int x, int y, SpriteID sprId);

    AssetManager mAsset;

private:
    bool loadAssets();
    void unloadAssets();
    void loadLevels();
    void _input(float dt);
    void update(float dt);
    void draw();
    // Runs input(), update() and draw()
    void tick();

    SDL_Window *mWindow = NULL;
    SDL_Renderer *mRenderer = NULL;
    TTF_Font *mFont = NULL;

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