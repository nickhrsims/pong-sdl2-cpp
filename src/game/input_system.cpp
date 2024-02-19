#include <algorithm>

#include <spdlog/spdlog.h>

#include "SDL_scancode.h"
#include "input_system.h"

static const std::string TAG{"Input Sub-system"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

InputSystem::InputSystem() {}
InputSystem::~InputSystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

void InputSystem::initialize(const Config& config) {
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

void InputSystem::terminate() {
    spdlog::info("Terminating {}.", TAG);
    actionToInputTypeMap.clear();
    actionToScancodeMap.clear();
    actionToMouseButtonMap.clear();
    std::fill_n(scancodeToActionMap, SDL_NUM_SCANCODES, Action::none);
    std::fill_n(buttonToActionMap, mouseButtonCount, Action::none);
}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
InputSystem& InputSystem::get() {
    static InputSystem instance{};
    return instance;
}

// -----------------------------------------------------------------------------
// Action Event Handling + Propogation
// -----------------------------------------------------------------------------

void InputSystem::handleKeyDownEvent(const SDL_KeyboardEvent& event) {
    (void)event;
    spdlog::debug("{} received Keyboard/KeyDown event", TAG);
}

void InputSystem::handleMouseButtonDownEvent(const SDL_MouseButtonEvent& event) {
    (void)event;
    spdlog::debug("{} received Mouse/Button event", TAG);
}

// -----------------------------------------------------------------------------
// Specialized Config Methods
// -----------------------------------------------------------------------------

void InputSystem::Config::setKeyboardKeyDownAction(SDL_Scancode scancode,
                                                   Action action) {
    actionMap[action].type              = InputType::keyboard;
    actionMap[action].keyboard.scancode = scancode;
}

void InputSystem::Config::setMouseButtonDownAction(uint8_t button, Action action) {
    actionMap[action].type         = InputType::mouse;
    actionMap[action].mouse.button = button;
}

// -----------------------------------------------------------------------------
// Action Queries
// -----------------------------------------------------------------------------

// HACK: The extra maps are redundant, just save the ActionDescription map.
bool InputSystem::isActionPressed(Action action) {
    switch (actionToInputTypeMap[action]) {
    case InputType::keyboard:
        return isKeyboardKeyDownActionPressed(action);
    case InputType::mouse:
        return isMouseButtonDownActionPressed(action);
    default:
        return false;
    }
}

bool InputSystem::isKeyboardKeyDownActionPressed(Action action) {
    SDL_Scancode scancode{actionToScancodeMap[action]};
    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);
    return keyboardState[scancode];
}

bool InputSystem::isMouseButtonDownActionPressed(Action action) {
    uint8_t button{actionToMouseButtonMap[action]};
    const uint32_t mouseState = SDL_GetMouseState(NULL, NULL);
    spdlog::debug("MouseState({}) | MouseButton({}) | MouseButtonMasked({})",
                  mouseState, button, SDL_BUTTON(button));
    return SDL_BUTTON(button) & mouseState;
}
