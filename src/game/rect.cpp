#include <algorithm>

#include "rect.h"

// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------

Rect::Rect() : SDL_Rect{0, 0, 0, 0} {}
Rect::Rect(int x, int y, int w, int h) : SDL_Rect{x, y, w, h} {}
Rect::Rect(const Rect& rhs) : SDL_Rect{rhs.x, rhs.y, rhs.w, rhs.h} {}
Rect::Rect(const Rect&& rhs) : SDL_Rect{rhs.x, rhs.y, rhs.w, rhs.h} {}

// -----------------------------------------------------------------------------
// Operators
// -----------------------------------------------------------------------------

// Copy Assignment
Rect& Rect::operator=(const Rect& rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->w = rhs.w;
    this->h = rhs.h;
    return *this;
}

// Move Assignment
Rect& Rect::operator=(const Rect&& rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->w = rhs.w;
    this->h = rhs.h;
    return *this;
}

// Sum (Minkowski Sum)
const Rect Rect::operator+(const Rect& rhs) const {
    return Rect{
        x + (rhs.x + rhs.w),
        y + (rhs.y + rhs.h),
        w + rhs.w,
        h + rhs.h,
    };
}

// Difference (Minkowski Difference)
const Rect Rect::operator-(const Rect& rhs) const {
    return Rect{
        x - (rhs.x + rhs.w),
        y - (rhs.y + rhs.h),
        w + rhs.w,
        h + rhs.h,
    };
}

// -----------------------------------------------------------------------------
// Public API Implementation
// -----------------------------------------------------------------------------

const Rect Rect::minkowskiDifference(const Rect& other) const { return *this - other; }

Vector2 Rect::getCenter() const { return Vector2{x + w / 2, y + h / 2}; }

bool Rect::hasPoint(int x, int y) const {
    int left   = x;
    int top    = y;
    int right  = x + w;
    int bottom = y + h;

    return (left < x && x < right && top < y && y < bottom);
}

Rect::Edge Rect::getIntersectingEdge(const Rect& other) const {
    Rect minkowski{minkowskiDifference(other)};

    // If the minkowski box surrounds the origin, then a collision has occured.
    bool isIntersecting{minkowski.hasPoint(0, 0)};

    int left   = std::abs(minkowski.x);
    int top    = std::abs(minkowski.y);
    int right  = std::abs(minkowski.x + minkowski.w);
    int bottom = std::abs(minkowski.y + minkowski.h);

    // TODO: Remove branch via. culling
    if (isIntersecting) {
        int closestEdge{std::min({left, right, top, bottom})};

        // TODO: Refactor to provide collision vector for simplified resolution.
        if (closestEdge == left)
            return Edge::left;
        if (closestEdge == right)
            return Edge::right;
        if (closestEdge == top)
            return Edge::top;
        if (closestEdge == bottom)
            return Edge::bottom;
    }

    return Edge::none; // Not intersecting
}
