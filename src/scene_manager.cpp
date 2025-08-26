#include "scene_manager.hpp"
#include "scene_boot.hpp"
#include "scene_isolevel.hpp"
#include "scene_leveledit.hpp"
#include "scene_splash.hpp"
#include "scene_main_menu.hpp"
#include "scene_options.hpp"
#include "scene_levelselect.hpp"
#include "scene_pause.hpp"
#include "scene_unity.hpp"
#include "scene_level.hpp"

std::unique_ptr<Scene> createScene(Scenes id)
{
    switch (id)
    {
    case Scenes::BOOT:
        return std::make_unique<BootScene>();
    case Scenes::SPLASH:
        return std::make_unique<SplashScene>();
    case Scenes::MAIN_MENU:
        return std::make_unique<MainMenuScene>();
    case Scenes::LEVEL_EDIT:
        return std::make_unique<LevelEditScene>();
    case Scenes::ISOLEVEL:
        return std::make_unique<IsoLevelScene>();
    case Scenes::OPTIONS:
        return std::make_unique<OptionsScene>();
    case Scenes::LEVEL_SELECT:
        return std::make_unique<LevelSelectScene>();
    case Scenes::PAUSE:
        return std::make_unique<PauseScene>();
    case Scenes::UNITY:
        return std::make_unique<UnityScene>();
    case Scenes::LEVEL:
        return std::make_unique<LevelScene>();
    default:
        return nullptr;
    }
}

void SceneManager::init(Scenes startScene)
{
    sceneStack.clear();
    sceneStack.push_back(createScene(startScene));
    if (sceneStack.back())
    {
        Log::debug("Scene manager init -- preload");
        sceneStack.back()->preload();
        sceneStack.back()->entities.removeAndAddEntities(true);
        sceneStack.back()->init();
    }
}

void SceneManager::requestSwitch(Scenes next)
{
    nextSceneId = next;
    doPush = false;
}

void SceneManager::pushScene(Scenes overlay)
{
    nextSceneId = overlay;
    doPush = true;
}

void SceneManager::popScene()
{
    doPop = true;
}

void SceneManager::update(float dt)
{
    performSceneChange();

    // if (transitionScene)
    // {
    //     transitionScene->entities.update(dt);
    //     // transitionScene->update(dt);
    //     return;
    // }

    if (!sceneStack.empty())
    {
        sceneStack.back()->entities.update(dt);
        sceneStack.back()->update(dt);
    }
}

void SceneManager::draw()
{
    for (auto& scene : sceneStack)
    {
        Game::BeginCamera();
        scene->entities.draw();
        scene->draw();
        Game::EndCamera();

        scene->entities.drawGUI();
        scene->drawGUI();
    }

    // if (transitionScene)
    // {
    //     transitionScene->entities.draw();
    //     // transitionScene->draw();
    // }
}

void SceneManager::dispose()
{
    while (!sceneStack.empty())
    {
        sceneStack.back()->dispose();
        sceneStack.pop_back();
    }

    // if (transitionScene) {
    //     transitionScene->dispose();
    //     transitionScene.reset();
    // }

    nextSceneId = Scenes::NONE;
}

void SceneManager::performSceneChange() {
    if (doPop && !sceneStack.empty()) {
        sceneStack.back()->dispose();
        sceneStack.pop_back();
        sceneStack.back()->onPopReturn();
        doPop = false;
    }

    if (nextSceneId != Scenes::NONE) {
        if (!doPush && !sceneStack.empty()) {
            sceneStack.back()->dispose();
            sceneStack.pop_back();
        }
        sceneStack.push_back(createScene(nextSceneId));
        Log::debug("Scene manager performSceneChange -- preload");
        sceneStack.back()->preload();
        sceneStack.back()->entities.removeAndAddEntities(true);
        sceneStack.back()->init();

        // Optional: Set transition overlay
        // transitionScene = std::make_unique<FadeTransition>();

        nextSceneId = Scenes::NONE;
        doPush = false;
    }
}
