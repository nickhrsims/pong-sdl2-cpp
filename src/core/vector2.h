#pragma once

/**
 * 2-Dimensional Vector.
 * Can be used as a point, velocity, size, etc.
 *
 * TODO: Tests
 */
struct Vector2 {
    int x;
    int y;

    Vector2();
    Vector2(int x, int y);

    bool isOrigin() const;
    bool operator_bool() const;
    Vector2 operator+(const Vector2& rhs) const;
    Vector2 operator-(const Vector2& rhs) const;
    Vector2 operator*(const Vector2& rhs) const;
    Vector2 operator/(const Vector2& rhs) const;
};
