#include "animation.hpp"
#include <SDL2/SDL.h>

void AnimationSprite::start()
{
    timer.setDuration(this->duration);
    timer.reset();
}

SpriteID AnimationSprite::tick()
{
    float elapsed = timer.elapsed();
    float perc = elapsed / duration;
    if (perc > 1.0f)
    {
        if (!loop)
            perc = 1.0f;
        else
        {
            float remaining = elapsed - duration;
            perc = remaining / duration;
            timer.reset();
            timer.forwardBy(remaining);
        }
    }

    float step = 1.0f / frameCount;

    if (reverse)
        perc = 1.0f - perc;

    int currFrame = (int)SDL_floorf(perc / step);
    if (currFrame > frameCount - 1)
        currFrame = frameCount - 1;

    return frames[currFrame];
}

bool AnimationSprite::isDone() const
{
    if (loop)
        return false;

    float elapsed = timer.elapsed();
    float perc = elapsed / duration;
    if (perc > 1.0f)
        return true;

    return false;
}
