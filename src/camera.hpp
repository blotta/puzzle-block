#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vmath.hpp"

struct Camera
{
    vec2 offset;
    vec2f target;
    vec2f pos;

    void update(float dt);
};

#endif
