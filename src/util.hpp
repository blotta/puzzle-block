#ifndef UTIL_HPP
#define UTIL_HPP

struct vec2
{
    int x = 0;
    int y = 0;
    vec2()
    {
        x = 0;
        y = 0;
    }
    vec2(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

#endif