#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "data.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "util.hpp"
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <functional>

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

template <typename T> using InterpolationFunction = std::function<T(const T&, const T&, float)>;

enum class InterpolationType
{
    LINEAR,
    EASE_IN,
    EASE_OUT
};

template <typename T> struct AnimationProperty
{
    void addKeyframe(float time, const T& value);
    template <size_t N> void addKeyframesEvenly(const T (&values)[N]);

    T evaluate(float t) const;

    std::vector<AnimationKeyframe<T>> keyframes;
    // InterpolationFunction<T> interpolationFunction = nullptr;
    InterpolationType interpolationType = InterpolationType::LINEAR;
};

template <typename T> void AnimationProperty<T>::addKeyframe(float time, const T& value)
{
    keyframes.push_back({time, value});
    std::sort(keyframes.begin(), keyframes.end(), [](const auto& a, const auto& b) { return a.time < b.time; });
}

template <typename T> template <size_t N> void AnimationProperty<T>::addKeyframesEvenly(const T (&values)[N])
{
    if (N == 0)
        return;

    float step = 1.0f / N;

    keyframes.clear();
    for (size_t i = 0; i < N; ++i)
    {
        float time = step * i;
        Log::debug("Adding keyframe %d at time: %f", i, time);
        keyframes.push_back({time, values[i]});
    }
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

class Animation
{
  public:
    float duration = 1.0f;
    float time = 0.0f;
    bool playing = false;
    AnimationPlayMode mode = AnimationPlayMode::ONCE;
    bool reverse = false;

    void start();

    void stop();

    void update(float dt);

    float getProgress() const;
};

#endif