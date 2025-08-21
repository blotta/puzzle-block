#include "animation.hpp"
#include <SDL2/SDL.h>
#include <algorithm>

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

///////////////
// ANIMATION //
///////////////

void Animation::addEvent(float progress, std::function<void()> action)
{
    events.push_back({progress, std::move(action), false});
}

void Animation::start()
{
    time = 0.0f;
    playing = true;
    reverse = false;
    for (auto& e : events)
        e.triggered = false;
}

void Animation::stop()
{
    playing = false;
}

void Animation::update(float dt)
{
    if (!playing)
        return;

    float delta = reverse ? -dt : dt;
    time += delta;

    float progress = getProgress();

    for (auto& e : events)
    {
        if (!e.triggered && progress >= e.triggerTime)
        {
            if (e.action)
                e.action();
            e.triggered = true;
        }
    }

    if (mode == AnimationPlayMode::ONCE)
    {
        if (time >= duration)
        {
            time = duration;
            playing = false;
            if (onComplete)
                onComplete();
        }
        else if (time < 0.0f)
        {
            time = 0.0f;
            playing = false;
        }
    }
    else if (mode == AnimationPlayMode::LOOP)
    {
        bool onCompleteCalled = false;
        while (time > duration)
        {
            time -= duration;
            if (!reverse && onComplete && !onCompleteCalled)
            {
                onComplete();
                onCompleteCalled = true;
            }
        }
        while (time < 0.0f)
        {
            time += duration;
            if (reverse && onComplete && !onCompleteCalled)
            {
                onComplete();
                onCompleteCalled = true;
            }
        }
    }
    else if (mode == AnimationPlayMode::PINGPONG)
    {
        if (time > duration)
        {
            time = duration;
            reverse = true;
        }
        else if (time < 0.0f)
        {
            time = 0.0f;
            reverse = false;
            if (onComplete)
            {
                onComplete();
            }
        }
    }
}

float Animation::getProgress() const
{
    return std::clamp(time / duration, 0.0f, 1.0f);
}
