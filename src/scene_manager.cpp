#include "scene_manager.hpp"
#include "scene_boot.hpp"
#include "scene_isolevel.hpp"
#include "scene_leveledit.hpp"
#include "scene_splash.hpp"
#include "scene_main_menu.hpp"

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
    default:
        return nullptr;
    }
}

void SceneManager::init(Scenes startScene)
{
    sceneStack.clear();
    sceneStack.push_back(createScene(startScene));
    if (sceneStack.back())
        sceneStack.back()->init();
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

    if (transitionScene)
    {
        transitionScene->update(dt);
        return;
    }

    if (!sceneStack.empty())
        sceneStack.back()->update(dt);
}

void SceneManager::draw()
{
    for (auto& scene : sceneStack)
        scene->draw();

    if (transitionScene)
        transitionScene->draw();
}

void SceneManager::dispose()
{
    while (!sceneStack.empty())
    {
        sceneStack.back()->dispose();
        sceneStack.pop_back();
    }

    if (transitionScene) {
        transitionScene->dispose();
        transitionScene.reset();
    }

    nextSceneId = Scenes::NONE;
}

void SceneManager::performSceneChange() {
    if (doPop && !sceneStack.empty()) {
        sceneStack.back()->dispose();
        sceneStack.pop_back();
        doPop = false;
    }

    if (nextSceneId != Scenes::NONE) {
        if (!doPush && !sceneStack.empty()) {
            sceneStack.back()->dispose();
            sceneStack.pop_back();
        }
        sceneStack.push_back(createScene(nextSceneId));
        sceneStack.back()->init();

        // Optional: Set transition overlay
        // transitionScene = std::make_unique<FadeTransition>();

        nextSceneId = Scenes::NONE;
        doPush = false;
    }
}
