#include "paddle.h"

Paddle::Paddle() : inputComponent{this}, renderComponent{this} { setSize(8, 64); }
void Paddle::update(float delta) {
    inputComponent.update(delta);
    move(delta);
}

void Paddle::draw() const { renderComponent.draw(); }
