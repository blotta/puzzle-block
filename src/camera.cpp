#include "camera.hpp"
#include <math.h>
#include <stdio.h>

#include "util.hpp"

void Camera::update(float dt)
{
    float step = 0.1f;

    pos = easings::lerp(pos, target, step);
}