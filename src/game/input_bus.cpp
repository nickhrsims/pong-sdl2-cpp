#include <algorithm>

#include <spdlog/spdlog.h>

#include "SDL_scancode.h"
#include "input_bus.h"

static const std::string TAG{"Input Sub-system"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

InputBus::InputBus() {}
InputBus::~InputBus() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

void InputBus::initialize(const Config& config) {
    spdlog::info("Initializing {}.", TAG);

    for (auto const& [action, description] : config.actionMap) {
        // Store the action's input type for later reference.
        actionToInputTypeMap[action] = description.type;

        // Assign action to appropriate input-type-to-action map.
        switch (description.type) {
        case InputType::keyboard:
            actionToScancodeMap[action] = description.keyboard.scancode;
            scancodeToActionMap[description.keyboard.scancode] = action;
            break;

        case InputType::mouse:
            actionToMouseButtonMap[action]              = description.mouse.button;
            buttonToActionMap[description.mouse.button] = action;
            break;

        default:
            spdlog::warn("{} configured with unknown input action type!", TAG);
            break;
        }
    }
}

void InputBus::terminate() {
    spdlog::info("Terminating {}.", TAG);
    actionToInputTypeMap.clear();
    actionToScancodeMap.clear();
    actionToMouseButtonMap.clear();
    std::fill_n(scancodeToActionMap, SDL_NUM_SCANCODES, Action::none);
    std::fill_n(buttonToActionMap, mouseButtonCount, Action::none);
}

void InputBus::reinitialize(const Config& config) {
    terminate();
    initialize(config);
}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
InputBus& InputBus::get() {
    static InputBus instance{};
    return instance;
}

// -----------------------------------------------------------------------------
// Raw Event Handling
// -----------------------------------------------------------------------------

void InputBus::handleKeyDownEvent(const SDL_KeyboardEvent& event) const {
    SDL_Scancode scancode{event.keysym.scancode};
    Action action{scancodeToActionMap[scancode]};

    // If no action assigned to scancode, ignore the event.
    if (action == Action::none) {
        return;
    }

    for (auto const& observer : observers) {
        observer(action);
    }
}

void InputBus::handleMouseButtonDownEvent(const SDL_MouseButtonEvent& event) const {
    uint8_t button{event.button};
    Action action{buttonToActionMap[button]};

    // If no action assigned to button, ignore the event.
    if (action == Action::none) {
        return;
    }

    for (auto const& observer : observers) {
        observer(action);
    }
}

// -----------------------------------------------------------------------------
// Action-Event Subscriptions
// -----------------------------------------------------------------------------

InputBus::Subscription
InputBus::onActionPressed(std::function<void(Action action)> callback) {
    return observers.insert(observers.end(), callback);
}

void InputBus::offActionPressed(Subscription subscription) {
    observers.erase(subscription);
}

// -----------------------------------------------------------------------------
// Config :: Input Action Configuration
// -----------------------------------------------------------------------------

void InputBus::Config::setKeyboardKeyDownAction(SDL_Scancode scancode, Action action) {
    actionMap[action].type              = InputType::keyboard;
    actionMap[action].keyboard.scancode = scancode;
}

void InputBus::Config::setMouseButtonDownAction(uint8_t button, Action action) {
    actionMap[action].type         = InputType::mouse;
    actionMap[action].mouse.button = button;
}

// -----------------------------------------------------------------------------
// Action Queries
// -----------------------------------------------------------------------------

bool InputBus::isActionPressed(Action action) const {
    switch (actionToInputTypeMap.at(action)) {
    case InputType::keyboard:
        return isKeyboardKeyDownActionPressed(action);
    case InputType::mouse:
        return isMouseButtonDownActionPressed(action);
    default:
        return false;
    }
}

bool InputBus::isKeyboardKeyDownActionPressed(Action action) const {
    SDL_Scancode scancode{actionToScancodeMap.at(action)};
    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);
    return keyboardState[scancode];
}

bool InputBus::isMouseButtonDownActionPressed(Action action) const {
    uint8_t button{actionToMouseButtonMap.at(action)};
    const uint32_t mouseState = SDL_GetMouseState(NULL, NULL);
    return SDL_BUTTON(button) & mouseState;
}
