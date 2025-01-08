#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "data.hpp"
#include "timer.hpp"

struct AnimationSprite
{
    float duration;
    int frameCount;
    SpriteID* frames;
    bool loop = false;

    Timer timer;

    void start();
    SpriteID tick();
};

#endif