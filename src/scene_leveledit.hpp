#ifndef SCENE_LEVELEDIT_HPP
#define SCENE_LEVELEDIT_HPP

#include "block.hpp"
#include "level.hpp"
#include "scene.hpp"

class LevelEditScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;

  private:
    void reset();
    void resize();
    void save(bool newLevel, bool saveToFile);
    void levelChanged();

    LevelVisual level;
    int lvlIdx;
    BlockVisual block;
    vec2 moveDir;
    vec2 mousePos;
    vec2 mouseIsoPos;
    bool saved = true;
    bool switchEditing = false;
    LevelSwitch tmpSwitch = {};
};

#endif