#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <string>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "scene.hpp"
#include "input_manager.hpp"

class Scene;
enum class Scenes;

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

    SDL_Renderer* getRenderer();
    SDL_Texture* getTexture(const char* textureName);
    TTF_Font* getFont();
    const std::string getState(const std::string& name);
    const std::string getOrCreateState(const std::string& name, const std::string& value);
    void setState(const std::string& name, const std::string& value);


private:

    bool init();
    bool loadAssets();
    void unloadAssets();
    void _input(float dt);
    void update(float dt);
    void draw();
    // Runs input(), update() and draw()
    void tick();

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