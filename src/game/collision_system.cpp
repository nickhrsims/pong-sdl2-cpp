#include <spdlog/spdlog.h>

#include "collision_system.h"

// NOTE: Very slow in theory, just fine for only a few entities.
void CollisionSystem::resolve(Rect& f, Paddle& lp, Paddle& rp, Ball& b) {

    // --- Left Paddle & Field
    if (lp.getTopEdgePosition() < f.y) {
        lp.setTopEdgePosition(f.y);
    } else if (lp.getBottomEdgePosition() > f.y + f.h) {
        lp.setBottomEdgePosition(f.y + f.h);
    }

    // --- Right Paddle & Field
    if (rp.getTopEdgePosition() < f.y) {
        rp.setTopEdgePosition(f.y);
    } else if (rp.getBottomEdgePosition() > f.y + f.h) {
        rp.setBottomEdgePosition(f.y + f.h);
    }

    // --- Ball & Field
    if (b.getTopEdgePosition() < f.y) {
        Vector2 v{b.getVelocity()};
        b.setVelocity(v.x, std::abs(v.y));

    } else if (b.getBottomEdgePosition() > f.y + f.h) {
        Vector2 v{b.getVelocity()};
        b.setVelocity(v.x, -std::abs(v.y));
    } else if (b.getLeftEdgePosition() < f.x) {
        // HACK: Temporary test impl
        Vector2 v{b.getVelocity()};
        b.setVelocity(std::abs(v.x), v.y);

    } else if (b.getRightEdgePosition() > f.x + f.w) {
        // HACK: Temporary test impl
        Vector2 v{b.getVelocity()};
        b.setVelocity(-std::abs(v.x), v.y);
    }

    // --- Ball & Left Paddle
    if ((lp.getRect() - b.getRect()).hasPoint(0, 0)) {
        // HACK: Temporary test impl
        Vector2 v{b.getVelocity()};
        b.setVelocity(std::abs(v.x), v.y);
    }

    // --- Ball & Right Paddle
    if ((rp.getRect() - b.getRect()).hasPoint(0, 0)) {
        // HACK: Temporary test impl
        Vector2 v{b.getVelocity()};
        b.setVelocity(-std::abs(v.x), v.y);
    }
}
