#include <SDL2/SDL.h>
#include "animation.hpp"

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

    int currFrame = (int)SDL_floorf(perc / step);
    if (currFrame > frameCount - 1)
        currFrame = frameCount - 1;

    return frames[currFrame];
}
