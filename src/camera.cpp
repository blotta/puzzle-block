#include "camera.hpp"
#include <math.h>
#include <stdio.h>

#include "util.hpp"

void Camera::update(float dt)
{
    float step = 0.1f;

    pos.x = linear_interpolate(pos.x, target.x, step);
    pos.y = linear_interpolate(pos.y, target.y, step);
}