#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "data.hpp"
#include "font.hpp"
#include "scene.hpp"
#include "scene_manager.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "camera.hpp"

class Game
{
  public:
    Game(const Game&) = delete;

    static int ScreenWidth();
    static int ScreenHeight();
    static int TargetFPS();
    static const GameSettings& Settings();

    static void Run();
    static void LoadScene(Scenes sceneName);
    static void PushScene(Scenes sceneName);
    static void PopScene();
    static void Exit();

    static SDL_Renderer* GetRenderer();

    static const std::string GetState(const std::string& name);
    static const std::string GetOrCreateState(const std::string& name, const std::string& value);
    static void SetState(const std::string& name, const std::string& value);

    static const int GetLevelsSize();
    static const std::vector<LevelData>& GetLevels();
    static const LevelData& GetLevelData(int idx);
    static void SaveLevelData(const LevelData& ld, int idx);
    static int AddLevelData(const LevelData& ld);

    static void CameraSetSize(const vec2& size);
    static void CameraSetTarget(const vec2f& target);
    static void CameraSetTarget(const vec2& target);
    static void CameraSetPosition(const vec2f& pos);
    static void CameraSetPosition(const vec2& pos);
    static vec2f ScreenToWorld(const vec2f& screenPos);
    static vec2f WorldToScreen(const vec2f& worldPos);
    static void BeginCamera();
    static void EndCamera();

    static void DrawPoint(int x, int y, SDL_Color color);
    static void DrawLine(int x1, int y1, int x2, int y2, SDL_Color color);

    static const Sprite& GetSprite(SpriteID id);
    static void DrawSprite(int x, int y, SpriteID sprId);

    static void PlaySound(const std::string& path);
    static void SetSoundVolume(int vol);
    static void PlayMusic(const std::string& path);
    static void SetMusicVolume(int vol);

    static void SetFont(const std::string& path, int ptsize);
    static void SetFontSize(int ptsize);
    static void Text(int x, int y, const std::string& text, const FontDrawOptions& = {});
    static int TextWidth(const std::string& text, const FontDrawOptions& = {});


  private:
    Game();
    ~Game();
    static Game& get();

    void loadAssets();
    void loadLevels();
    void init();
    void run();
    void dispose();

    // Runs update() and draw()
    void tick();

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
    const Font* mActiveFont;
    Camera mCamera;
    bool mCameraEnabled = false;
    vec2f getCameraDiff(bool asEnabled);

#if DEBUG
    Scenes mStartScene = Scenes::BOOT;
#else
    Scenes mStartScene = Scenes::SPLASH;
#endif
    SceneManager mSceneManager;

    std::map<std::string, std::string> mState;

    std::vector<LevelData> mLevels;

    // data
    GameData mData = {};

    GameSettings settings;
};

#endif
