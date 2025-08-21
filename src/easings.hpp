#ifndef EASINGS_HPP
#define EASINGS_HPP

#include "vmath.hpp"
#include "data.hpp"

namespace easings
{
// linear
float lerp(float a, float b, float t);
int lerp(int a, int b, float t);
vec2f lerp(const vec2f& a, const vec2f& b, float t);
vec2 lerp(const vec2& a, const vec2& b, float t);
SpriteID lerp(const SpriteID& a, const SpriteID& b, float t);

// ease in
float easeIn(float a, float b, float t);
int easeIn(int a, int b, float t);
vec2f easeIn(const vec2f& a, const vec2f& b, float t);
vec2 easeIn(const vec2& a, const vec2& b, float t);
SpriteID easeIn(const SpriteID& a, const SpriteID& b, float t);

// ease out
float easeOut(float a, float b, float t);
int easeOut(int a, int b, float t);
vec2f easeOut(const vec2f& a, const vec2f& b, float t);
vec2 easeOut(const vec2& a, const vec2& b, float t);
SpriteID easeOut(const SpriteID& a, const SpriteID& b, float t);

// ease out bounce
float easeOutBounce(float a, float b, float t);
int easeOutBounce(int a, int b, float t);
vec2f easeOutBounce(const vec2f& a, const vec2f& b, float t);
vec2 easeOutBounce(const vec2& a, const vec2& b, float t);
SpriteID easeOutBounce(const SpriteID& a, const SpriteID& b, float t);

// easeOutBack functions
float easeOutBack(float a, float b, float t);
int easeOutBack(int a, int b, float t);
vec2f easeOutBack(const vec2f& a, const vec2f& b, float t);
vec2 easeOutBack(const vec2& a, const vec2& b, float t);
SpriteID easeOutBack(const SpriteID& a, const SpriteID& b, float t);

// snap functions
float snap(float a, float b, float t) ;
int snap(int a, int b, float t);
vec2f snap(const vec2f& a, const vec2f& b, float t);
vec2 snap(const vec2& a, const vec2& b, float t);
SpriteID snap(const SpriteID& a, const SpriteID& b, float t);


} // namespace easings

#endif