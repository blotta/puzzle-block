#include "timer.hpp"

#include <SDL2/SDL.h>

Timer::Timer()
{
    reset();
}

Timer::Timer(float duration)
{
    mDuration = duration;
}

Timer::~Timer()
{
}

/// @brief duration in seconds
/// @param duration 
void Timer::setDuration(float duration)
{
    mDuration = duration;
}

float Timer::elapsed() const
{
    int currentTicks = SDL_GetTicks();
    return (currentTicks - mStartTicks) / 1000.0f;
}

void Timer::reset()
{
    mEndTicks = mStartTicks;
    mStartTicks = SDL_GetTicks();
}

void Timer::mark()
{
    mEndTicks = mStartTicks;
}

bool Timer::isDone() const
{
    float remaining = mDuration - elapsed();
    return remaining <= 0;
}

float Timer::waitUntilDone() const
{
    float remaining = mDuration - elapsed();
    if (remaining > 0)
    {
        SDL_Delay((Uint32)(remaining * 1000));
    }
    return remaining;
}
