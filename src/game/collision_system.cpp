#include "collision_system.h"

// NOTE: Very slow in theory, just fine for only a few entities.
void CollisionSystem::resolve(Rect& f, Paddle& lp, Paddle& rp) {

    // --- Field & Left Paddle
    if (lp.getTopEdgePosition() < f.y) {
        lp.setTopEdgePosition(f.y);
    } else if (lp.getBottomEdgePosition() > f.y + f.h) {
        lp.setBottomEdgePosition(f.y + f.h);
    }

    // --- Field & Right Paddle
    if (rp.getTopEdgePosition() < f.y) {
        rp.setTopEdgePosition(f.y);
    } else if (rp.getBottomEdgePosition() > f.y + f.h) {
        rp.setBottomEdgePosition(f.y + f.h);
    }
}
