#pragma once

#include <SDL_rect.h>

struct Vector2 {
    int x;
    int y;
};

struct Vector4 : SDL_Rect {};

class Entity {
  public:
    struct Velocity : Vector2 {};

    struct AABB : Vector4 {
        enum class Edge {
            none,
            left,
            top,
            right,
            bottom,
        };
    };

    virtual void update(float delta) = 0;

    // ---------------------------------
    // Position
    // ---------------------------------

    const Vector2 getPosition();
    void setPosition(int x, int y);

    int getLeftEdgePosition();
    void setLeftEdgePosition(int x);

    int getTopEdgePosition();
    void setTopEdgePosition(int y);

    int getRightEdgePosition();
    void setRightEdgePosition(int x);

    int getBottomEdgePosition();
    void setBottomEdgePosition(int y);

    // ---------------------------------
    // Size
    // ---------------------------------

    const Vector2 getSize();
    void setSize(int w, int h);

    // ---------------------------------
    // Velocity
    // ---------------------------------

    const Vector2 getVelocity();
    void setVelocity(int vx, int vy);

    void move(float delta);

  private:
    Velocity velocity{0, 0};
    AABB aabb{0, 0, 0, 0};
};