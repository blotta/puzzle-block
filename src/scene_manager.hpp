#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "Scene.hpp"
#include <memory>
#include <vector>

class SceneManager
{
  public:
    void init(Scenes startScene);
    void update(float dt);
    void draw();
    void dispose();

    void requestSwitch(Scenes next);
    void pushScene(Scenes overlay);
    void popScene();

  private:
    void performSceneChange();

    std::vector<std::unique_ptr<Scene>> sceneStack;
    std::unique_ptr<Scene> transitionScene;

    Scenes nextSceneId = Scenes::NONE;
    bool doPush = false;
    bool doPop = false;
};

#endif // SCENE_MANAGER_HPP