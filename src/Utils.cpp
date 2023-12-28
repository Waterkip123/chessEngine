#include "Utils.hpp"

Vec2::Vec2() {}
Vec2::Vec2(int8_t x, int8_t y) : x(x), y(y) {}

Vec2 Vec2::operator+(Vec2 rhs)
{
    Vec2 result = Vec2(x + rhs.x, y + rhs.y);
    return result;
}

Vec2 Vec2::operator*(int rhs)
{
    Vec2 result = Vec2(x * rhs, y * rhs);
    return result;
}

bool Vec2::operator==(Vec2 rhs)
{
    bool result = (x == rhs.x) && (y == rhs.y);
    return result;
}