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
