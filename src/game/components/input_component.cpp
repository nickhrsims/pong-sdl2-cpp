#include "input_component.h"
#include "game/input_system.h"

static const int SPEED = 400;

InputComponent::InputComponent(Entity* entity, Player player) : entity{entity} {
    switch (player) {
    case Player::one:
        upAction   = InputSystem::Action::playerOneUp;
        downAction = InputSystem::Action::playerOneDown;
        break;
    case Player::two:
        upAction   = InputSystem::Action::playerTwoUp;
        downAction = InputSystem::Action::playerTwoDown;
    }
}

void InputComponent::update(float delta) const {
    (void)delta;
    InputSystem& input{InputSystem::get()};

    bool up{input.isActionPressed(upAction)};
    bool down{input.isActionPressed(downAction)};

    entity->setVelocity(0, (down - up) * SPEED);
}
