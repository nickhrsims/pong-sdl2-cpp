#include "ball.h"

Ball::Ball() : renderComponent{this} { setSize(8, 8); }

void Ball::update(float delta) { move(delta); }

void Ball::draw() const { renderComponent.draw(); }
