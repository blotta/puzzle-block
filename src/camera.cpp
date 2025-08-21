#include "camera.hpp"
#include "easings.hpp"
#include <math.h>
#include <stdio.h>

void Camera::update(float dt)
{
    float step = 0.1f;

    pos = easings::lerp(pos, target, step);
}