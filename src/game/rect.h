#pragma once

#include "SDL_rect.h"

/**
 *
 * A.k.a "Axis-Aligned Bounding Box", and all that implies.
 *
 * A simple wrapper around SDL_Rect.
 * Rect specialization for position, size, and collision geometry.
 *
 * The "origin" from the perspective of the API is the center of the Rect.
 */
struct Rect : public SDL_Rect {
    Rect();
    Rect(int x, int y, int w, int h);

    Rect(const Rect& rhs);
    Rect(const Rect&& rhs);

    Rect& operator=(const Rect& rhs);
    Rect& operator=(const Rect&& rhs);

    /**
     * Returns the Minkowski Sum between Rects.
     */
    const Rect operator+(const Rect& rhs) const;

    /**
     * Returns the Minkowski Difference between Rects.
     */
    const Rect operator-(const Rect& rhs) const;

    /**
     * Describes an edge of an AABB.
     */
    enum class Edge {
        none,
        left,
        top,
        right,
        bottom,
    };

    /**
     * The geometric spatial-difference between two convex polygons.
     * Carries some interesting properties that allow it to be used for
     * collision detection.
     */
    const Rect minkowskiDifference(const Rect& other) const;

    /**
     * Does the given point line within the boundaries of this Rect?
     */
    bool hasPoint(int x, int y) const;

    /**
     * Return the edge of `this` most overlapped by the `other` Rect.
     * Will return `Edge::none` of no overlap occurs.
     *
     * Can be used to determine the most likely direction of "incoming"
     * collisions.
     */
    Edge getIntersectingEdge(const Rect& other) const;
};
