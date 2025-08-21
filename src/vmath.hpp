#ifndef VMATH_HPP
#define VMATH_HPP

struct vec2
{
    int x, y;
    vec2() : x(0), y(0)
    {
    }
    vec2(int x, int y) : x(x), y(y)
    {
    }

    vec2 operator+(const vec2& other) const
    {
        return vec2(this->x + other.x, this->y + other.y);
    }
    vec2 operator-(const vec2& other) const
    {
        return vec2(this->x - other.x, this->y - other.y);
    }
    vec2 operator*(int scalar) const
    {
        return vec2(this->x * scalar, this->y * scalar);
    }
    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

struct vec2f
{
    float x, y;
    vec2f() : x(0), y(0)
    {
    }
    vec2f(float x, float y) : x(x), y(y)
    {
    }
    vec2f(const vec2& vi) : x(static_cast<float>(vi.x)), y(static_cast<float>(vi.y))
    {
    }
    vec2f(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y))
    {
    }

    vec2f operator+(const vec2f& other) const
    {
        return vec2f(this->x + other.x, this->y + other.y);
    }
    vec2f operator-(const vec2f& other) const
    {
        return vec2f(this->x - other.x, this->y - other.y);
    }
    vec2f operator*(float scalar) const
    {
        return vec2f(this->x * scalar, this->y * scalar);
    }

    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};


#endif