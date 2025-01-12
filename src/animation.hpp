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
    bool reverse = false;

    Timer timer;

    void start();
    SpriteID tick();
    bool isDone() const;
};

#endif