#ifndef SCENE_OPTIONS_HPP
#define SCENE_OPTIONS_HPP

#include "scene.hpp"

class OptionsScene : public Scene
{
  public:
    void init() override;
    void update(float dt) override;
    void draw() override;
    void dispose() override;

  private:
    int mCursor = 1;
    int mLines = 2; // sfx, music
};

#endif