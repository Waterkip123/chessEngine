#pragma once

#include <cstdint>

struct Vec2
{
    Vec2();
    Vec2(int8_t x, int8_t y);

    int8_t x;
    int8_t y;

    Vec2 operator+(Vec2 rhs);
    Vec2 operator*(int rhs);
    bool operator==(Vec2 rhs);
};
