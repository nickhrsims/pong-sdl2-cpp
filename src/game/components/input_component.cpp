#include "input_component.h"

static const int SPEED = 400;

InputComponent::InputComponent(Entity* entity) : entity{entity} {}

void InputComponent::update(float delta) const {
    (void)delta;
    using Action = InputSystem::Action;
    InputSystem& input{InputSystem::get()};

    bool up{input.isActionPressed(Action::playerOneUp)};
    bool down{input.isActionPressed(Action::playerOneDown)};

    entity->setVelocity(0, (down - up) * SPEED);
}
