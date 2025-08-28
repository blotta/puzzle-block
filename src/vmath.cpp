#include "vmath.hpp"

#include <math.h>

vec2 vec2::operator+(const vec2& other) const
{
    return vec2(this->x + other.x, this->y + other.y);
}
vec2 vec2::operator-(const vec2& other) const
{
    return vec2(this->x - other.x, this->y - other.y);
}
vec2 vec2::operator*(int scalar) const
{
    return vec2(this->x * scalar, this->y * scalar);
}
bool vec2::operator==(const vec2& other) const
{
    return this->x == other.x && this->y == other.y;
}

int vec2::length() const
{
    return std::sqrt(x * x + y * y);
}

vec2f vec2f::operator+(const vec2f& other) const
{
    return vec2f(this->x + other.x, this->y + other.y);
}
vec2f vec2f::operator-(const vec2f& other) const
{
    return vec2f(this->x - other.x, this->y - other.y);
}
vec2f vec2f::operator*(float scalar) const
{
    return vec2f(this->x * scalar, this->y * scalar);
}

bool vec2f::operator==(const vec2& other) const
{
    return this->x == other.x && this->y == other.y;
}
void vec2f::operator+=(const vec2f& other)
{
    this->x += other.x;
    this->y += other.y;
}

float vec2f::length() const
{
    return std::sqrt(x * x + y * y);
}

void vec2f::normalize()
{
    float len = length();
    if (len > 0.0f)
    {
        x /= len;
        y /= len;
    }
}

vec2f vec2f::normalized() const
{
    float len = length();
    if (len > 0.0f)
    {
        return vec2f(x / len, y / len);
    }
    return vec2f(0, 0);
}

vec2 Rect::center() const
{
    return vec2(x + w / 2, y + h / 2);
}

vec2 Rect::pos() const
{
    return vec2(x, y);
}

bool Rect::contains(int px, int py) const
{
    return px >= x && px < x + w && py >= y && py < y + h;
}

bool Rect::contains(const vec2& pos) const
{
    return this->contains(pos.x, pos.y);
}

int Rect::right() const
{
    return x + w;
}

int Rect::bottom() const
{
    return y + h;
}

void Rect::clampInside(const Rect& parent)
{
    if (right() > parent.right())
        x = parent.right() - w;
    if (x < parent.x)
        x = parent.x;
    if (bottom() > parent.bottom())
        y = parent.bottom() - h;
    if (y < parent.y)
        y = parent.y;
}
