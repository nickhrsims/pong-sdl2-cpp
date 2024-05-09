#include "ball.h"

#include "core/color.h"
#include "core/renderer.h"

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
Ball::Ball() { setSize(8, 8); }

// -----------------------------------------------------------------------------
// Static Function Components
// -----------------------------------------------------------------------------
static void renderWhiteRect(Rect const& rect) {
    static auto const& renderer{Renderer::get()};
    renderer.drawRect(rect, Color::white());
}

// -----------------------------------------------------------------------------
// Entity Overrides
// -----------------------------------------------------------------------------
void Ball::update(float delta) { move(delta); }
void Ball::draw() const { renderWhiteRect(getRect()); }

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------
void Ball::randomizeVelocity() {
    double degrees = 80 * ((double)rand() / (double)RAND_MAX - 0.5) +
                     (10 * ((double)rand() / (double)RAND_MAX));
    double radians = degrees * M_PI / 180;

    int xDirection = ((float)rand() / (float)RAND_MAX) < 0.5 ? -1 : 1;
    int yDirection = ((float)rand() / (float)RAND_MAX) < 0.5 ? -1 : 1;

    int vx = (int)floor(cos(radians) * xDirection * defaultSpeed);
    int vy = (int)floor(sin(radians) * yDirection * defaultSpeed);
    setVelocity(vx, vy);
}
