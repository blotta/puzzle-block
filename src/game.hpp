#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <string>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "scene.hpp"

class Scene;
enum class Scenes;

class Game
{
public:
    bool isRunning();

    bool init();
    void run();

    bool loadAssets();
    void unloadAssets();
    void loadScene(Scenes sceneName);

    void update(float dt);
    void draw();
    void input();

    // Runs input(), update() and draw()
    void tick();

    SDL_Renderer* getRenderer();
    SDL_Texture* getTexture(const char* textureName);
    TTF_Font* getFont();
    const std::string getState(const std::string& name);
    const std::string getOrCreateState(const std::string& name, const std::string& value);
    void setState(const std::string& name, const std::string& value);

    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    const int TargetFPS = 60;

    Game();

    ~Game();
private:
    SDL_Window *mWindow = NULL;
    SDL_Renderer *mRenderer = NULL;
    TTF_Font* mFont = NULL;

    bool mInitialized = false;
    bool mRunning = false;

    std::map<std::string,SDL_Texture*> mTextures;

    std::shared_ptr<Scene> mCurrentScene;
    std::map<std::string,std::string> mState;
};

#endif