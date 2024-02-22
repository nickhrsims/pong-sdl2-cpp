#include "vector2.h"

Vector2::Vector2() : x{0}, y{0} {}

Vector2::Vector2(int x, int y) : x{x}, y{y} {}

bool Vector2::isOrigin() const { return !(x || y); }

bool Vector2::operator_bool() const { return x || y; }

Vector2 Vector2::operator+(const Vector2& rhs) const {
    return Vector2{x + rhs.x, y + rhs.y};
}

Vector2 Vector2::operator-(const Vector2& rhs) const {
    return Vector2{x - rhs.x, y - rhs.y};
}

Vector2 Vector2::operator*(const Vector2& rhs) const {
    return Vector2{x * rhs.x, y * rhs.y};
}

Vector2 Vector2::operator/(const Vector2& rhs) const {
    return Vector2{x / rhs.x, y / rhs.y};
}
