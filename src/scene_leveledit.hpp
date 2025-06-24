#ifndef SCENE_LEVEL_HPP
#define SCENE_LEVEL_HPP

#include "block.hpp"
#include "level.hpp"
#include "scene.hpp"
#include "text.hpp"

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

    Level level;
    int lvlIdx;
    Block block;
    int offsetX = 100;
    int offsetY = 100;
    int cellSize = 64;
    vec2 moveDir;
    vec2 mousePos;
    vec2 mouseIsoPos;
    bool saved = true;
    StaticText mEditInstruction;
    StaticText mLevelText;
    bool switchEditing = false;
    LevelSwitch tmpSwitch = {};
};

#endif