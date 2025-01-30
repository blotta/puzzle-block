#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "util.hpp"

struct Camera {
    vec2 offset;
    vec2f target;
    vec2f pos;

    void update(float dt);
};

#endif
