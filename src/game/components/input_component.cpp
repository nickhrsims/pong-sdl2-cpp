#include "input_component.h"
#include "game/input_bus.h"

static const int SPEED = 400;

InputComponent::InputComponent(Entity* entity, Player player) : entity{entity} {
    switch (player) {
    case Player::one:
        upAction   = InputBus::Action::playerOneUp;
        downAction = InputBus::Action::playerOneDown;
        break;
    case Player::two:
        upAction   = InputBus::Action::playerTwoUp;
        downAction = InputBus::Action::playerTwoDown;
    }
}

void InputComponent::update(float delta) const {
    (void)delta;
    InputBus& input{InputBus::get()};

    bool up{input.isActionPressed(upAction)};
    bool down{input.isActionPressed(downAction)};

    entity->setVelocity(0, (down - up) * SPEED);
}
