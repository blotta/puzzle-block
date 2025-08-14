#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <span>
// #include <map>
// #include <string>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <vector>

#include "data.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "util.hpp"

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

template <typename T> struct AnimationKeyframe
{
    float time; // 0.0 to 1.0
    T value;
};

enum class InterpolationType
{
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_OUT_BOUNCE,
    EASE_OUT_BACK,
    SNAP,
};

template <typename T> struct AnimationProperty
{
    void addKeyframe(float time, const T& value);

    void addKeyframesEvenly(std::span<const T> values, bool lastAtEnd = false)
    {
        keyframes.clear();

        size_t count = values.size();
        if (count == 0)
            return;

        if (count == 1)
        {
            addKeyframe(0.0f, values[0]);
            return;
        }

        size_t denom = lastAtEnd ? count - 1 : count;

        size_t i = 0;
        for (const auto& value : values)
        {
            float t = static_cast<float>(i) / static_cast<float>(denom);
            if (t > 1.0f)
                t = 1.0f;
            Log::debug("Adding keyframe %d at time: %f", i, t);
            addKeyframe(t, value);
            ++i;
        }
    }

    void addKeyframesEvenly(std::initializer_list<T> values, bool lastAtEnd = false)
    {
        addKeyframesEvenly(std::span<const T>(values.begin(), values.size()), lastAtEnd);
    }

    T evaluate(float t) const;

    std::vector<AnimationKeyframe<T>> keyframes;
    InterpolationType interpolationType = InterpolationType::LINEAR;
};

template <typename T> void AnimationProperty<T>::addKeyframe(float time, const T& value)
{
    keyframes.push_back({time, value});
    std::sort(keyframes.begin(), keyframes.end(), [](const auto& a, const auto& b) { return a.time < b.time; });
}

template <typename T> T AnimationProperty<T>::evaluate(float t) const
{
    if (keyframes.empty())
        return T{};
    if (t <= keyframes.front().time)
        return keyframes.front().value;
    if (t >= keyframes.back().time)
        return keyframes.back().value;

    for (size_t i = 0; i < keyframes.size() - 1; ++i)
    {
        const auto& k0 = keyframes[i];
        const auto& k1 = keyframes[i + 1];
        if (t >= k0.time && t <= k1.time)
        {
            float localT = (t - k0.time) / (k1.time - k0.time);
            if (interpolationType == InterpolationType::LINEAR)
                return easings::lerp(k0.value, k1.value, localT);
            else if (interpolationType == InterpolationType::EASE_IN)
                return easings::easeIn(k0.value, k1.value, localT);
            else if (interpolationType == InterpolationType::EASE_OUT)
                return easings::easeOut(k0.value, k1.value, localT);
            else if (interpolationType == InterpolationType::EASE_OUT_BOUNCE)
                return easings::easeOutBounce(k0.value, k1.value, localT);
            else if (interpolationType == InterpolationType::EASE_OUT_BACK)
                return easings::easeOutBack(k0.value, k1.value, localT);
            else if (interpolationType == InterpolationType::SNAP)
                return easings::snap(k0.value, k1.value, localT);
            else
                return T{};
        }
    }

    return keyframes.back().value;
}

enum class AnimationPlayMode
{
    ONCE,
    LOOP,
    PINGPONG
};

struct AnimationEvent
{
    float triggerTime;
    std::function<void()> action;
    bool triggered = false;
};

class Animation
{
  public:
    float duration = 1.0f;
    float time = 0.0f;
    bool playing = false;
    AnimationPlayMode mode = AnimationPlayMode::ONCE;
    bool reverse = false;

    std::function<void()> onComplete = nullptr;
    std::vector<AnimationEvent> events;

    void addEvent(float progress, std::function<void()> action);

    void start();

    void stop();

    void update(float dt);

    float getProgress() const;
};

#endif
