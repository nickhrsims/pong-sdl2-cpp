#include "rect.h"
#include <algorithm>

const Rect Rect::minkowskiDifference(const Rect& other) const {
    return Rect{
        x - (other.x + other.w),
        y - (other.y + other.h),
        w + other.w,
        h + other.h,
    };
}

const Rect Rect::operator-(const Rect& rhs) const { return minkowskiDifference(rhs); }

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
