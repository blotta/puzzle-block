#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "scene.hpp"

class Scene;
enum class Scenes;

class Game
{
public:
    bool isRunning();
    bool init();
    bool loadAssets();
    void unloadAssets();
    void loadScene(Scenes sceneName);

    void run();
    void update();
    void draw();
    void input();

    // Runs input(), update() and draw()
    void tick();

    SDL_Renderer* getRenderer();
    SDL_Texture* getTexture(const char* textureName);

    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    const int TargetFPS = 60;

    Game();

    ~Game();
private:
    SDL_Window *mWindow = NULL;
    SDL_Renderer *mRenderer = NULL;

    bool mInitialized = false;
    bool mRunning = false;

    std::map<std::string,SDL_Texture*> mTextures;

    Scene* mCurrentScene = nullptr;
};

#endif