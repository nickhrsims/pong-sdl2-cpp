#include "ball.h"

Ball::Ball() : renderComponent{this} { setSize(8, 8); }

void Ball::update(float delta) { move(delta); }

void Ball::draw() const { renderComponent.draw(); }

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
