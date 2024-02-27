#pragma once

#include "SDL_rect.h"

#include "core/vector2.h"

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
     * Get center position.
     */
    Vector2 getCenter() const;

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

    /**
     * A vertical slice of a `Rect`.
     *
     * `ratio` - number of slices
     * `index` - specific equal-width slice.
     *
     * Each paddle (left or right) determines where each
     * field section begins horizontally.
     *
     * Examples
     * --------
     *
     *   First slice (i.e. index 0):
     *     Slice occupies this space on the left end of the rect.
     *    ----------------
     *   |XXX|            |
     *   |XXX|            |
     *   |XXX|            |
     *    ----------------
     *
     *   Last slice (i.e. index == (sliceCount - 1)):
     *     Slice occupies this space on the right end of the rect.
     *    ----------------
     *   |            |XXX|
     *   |            |XXX|
     *   |            |XXX|
     *    ----------------
     *
     * The specific width of the slice is determined
     * by the given `ratio`.
     *
     *
     * This slice is used to simplify positioning logic.
     */

    Rect getVerticalSlice(unsigned int sliceCount, unsigned int index) const;
};
