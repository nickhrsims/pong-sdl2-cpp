#include <algorithm>

#include "entity.h"

const Vector2 Entity::getPosition() const {
    return Vector2{aabb.x - (aabb.w / 2), aabb.y - (aabb.h / 2)};
}

void Entity::setPosition(int x, int y) {
    aabb.x = x - (aabb.w / 2);
    aabb.y = y - (aabb.h / 2);
}

int Entity::getLeftEdgePosition() const { return aabb.x; }
void Entity::setLeftEdgePosition(int x) { aabb.x = x; }

int Entity::getTopEdgePosition() const { return aabb.y; }
void Entity::setTopEdgePosition(int y) { aabb.y = y; }

int Entity::getRightEdgePosition() const { return aabb.x + aabb.w; }
void Entity::setRightEdgePosition(int x) { aabb.x = x - aabb.w; }

int Entity::getBottomEdgePosition() const { return aabb.y + aabb.h; }
void Entity::setBottomEdgePosition(int y) { aabb.y = y - aabb.h; }

const Vector2 Entity::getSize() const { return Vector2{aabb.w, aabb.h}; }
void Entity::setSize(int w, int h) {
    aabb.w = w;
    aabb.h = h;
}

const Vector2 Entity::getVelocity() const { return velocity; }
void Entity::setVelocity(int vx, int vy) {
    velocity.x = vx;
    velocity.y = vy;
}

void Entity::move(float delta) {
    aabb.x += velocity.x * delta;
    aabb.y += velocity.y * delta;
}

const Rect Entity::getRect() const { return aabb; }

// -----------------------------------------------------------------------------
// AABB
// -----------------------------------------------------------------------------

const Rect Entity::AABB::minkowskiDifference(const AABB& other) const {
    return Rect{
        x - (other.x + other.w),
        y - (other.y + other.h),
        w + other.w,
        h + other.h,
    };
}

const Rect Entity::AABB::operator-(const AABB& rhs) const {
    return minkowskiDifference(rhs);
}

bool Entity::AABB::hasPoint(int x, int y) const {
    int left   = x;
    int top    = y;
    int right  = x + w;
    int bottom = y + h;

    return (left < x && x < right && top < y && y < bottom);
}

Entity::AABB::Edge Entity::AABB::getIntersectingEdge(const Entity::AABB& other) const {
    AABB result{minkowskiDifference(other)};

    // If the minkowski box surrounds the origin, then a collision has occured.
    bool isIntersecting{result.hasPoint(0, 0)};

    int left   = std::abs(result.x);
    int top    = std::abs(result.y);
    int right  = std::abs(result.x + result.w);
    int bottom = std::abs(result.y + result.h);

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
