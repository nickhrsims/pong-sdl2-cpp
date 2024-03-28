#pragma once

#include "core/rect.h"
#include "core/vector2.h"

// TODO: Tests
// TODO: Docs
class Entity {
  public:
    /**
     * A Vector2 specialization for velocity.
     * Presently doesn't add anything new, here to support refactoring.
     */
    struct Velocity : public Vector2 {
        using Vector2::Vector2;
    };

    /**
     * Override this to describe how the entity is to behave during the
     * "update" phase.
     */
    virtual void update(float delta) = 0;

    /**
     * Override this to describe how the entity is to behave during the
     * "rendering" phase.
     */
    virtual void draw() const = 0;

    // ---------------------------------
    // Position
    // ---------------------------------

    /**
     * Get a constant `Vector2` describing the position of the entity.
     */
    const Vector2 getPosition() const;

    /**
     * Set the position of the entity.
     */
    void setPosition(int x, int y);

    /**
     * Get the horizontal position of the entity's left edge.
     */
    int getLeftEdgePosition() const;

    /**
     * Set the horizontal position of the entity's left edge.
     */
    void setLeftEdgePosition(int x);

    /**
     * Get the vertical position of the entity's top edge.
     */
    int getTopEdgePosition() const;

    /**
     * Set the vertical position of the entity's top edge.
     */
    void setTopEdgePosition(int y);

    /**
     * Get the horizontal position of the entity's right edge.
     */
    int getRightEdgePosition() const;

    /**
     * Set the horizontal position of the entity's right edge.
     */
    void setRightEdgePosition(int x);

    /**
     * Get the vertical position of the entity's bottom edge.
     */
    int getBottomEdgePosition() const;

    /**
     * Set the vertical position of the entity's bottom edge.
     */
    void setBottomEdgePosition(int y);

    // ---------------------------------
    // Size
    // ---------------------------------

    /**
     * Get a constant `Vector2` of the entity's size.
     */
    const Vector2 getSize() const;

    /**
     * Set the entity's size.
     */
    void setSize(int w, int h);

    // ---------------------------------
    // Velocity
    // ---------------------------------

    /**
     * Get a constant `Vector2` of the entity's velocity.
     */
    const Vector2 getVelocity() const;

    /**
     * Set the entity's velocity.
     */
    void setVelocity(int vx, int vy);

    /**
     * Change the entity's position based on it's current velocity.
     */
    void move(float delta);

    // ---------------------------------
    // Rect Access
    // ---------------------------------

    /**
     * Get a constant `Rect` describing the entity's geometry.
     */
    const Rect getRect() const;

  private:
    Velocity velocity{0, 0};
    Rect aabb{0, 0, 0, 0};
};
