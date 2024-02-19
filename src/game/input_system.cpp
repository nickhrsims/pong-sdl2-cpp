#include <spdlog/spdlog.h>

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

// FIXME: Not Implemented
void InputSystem::initialize(const Config& config) {
    (void)config;
    spdlog::info("Initializing {}.", TAG);
}

// FIXME: Not Implemented
void InputSystem::terminate() { spdlog::info("Terminating {}.", TAG); }

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
InputSystem& InputSystem::get() {
    static InputSystem instance{};
    return instance;
}

// -----------------------------------------------------------------------------
// Event Handling + Specialized Input Event Mapping
// -----------------------------------------------------------------------------

void handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        SDL_Scancode scancode = event.key.keysym.scancode;
        InputSystem::get();
    }
}
