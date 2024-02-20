#include <string>

#include <spdlog/spdlog.h>

#include "SDL.h"
#include "display_system.h"

static const std::string TAG{"Display Sub-system"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

DisplaySystem::DisplaySystem() {}
DisplaySystem::~DisplaySystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

void DisplaySystem::initialize(const Config& config) {
    spdlog::info("Initializing {}.", TAG);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    // TODO: support flags (fullscreen etc.)
    window = SDL_CreateWindow(config.windowTitle.c_str(), config.windowPositionX,
                              config.windowPositionY, config.windowWidth,
                              config.windowHeight, 0);
    if (!window) {
        spdlog::error("{} Error: Failed to create window!");
    }
}

void DisplaySystem::terminate() {
    spdlog::info("Terminating {}.", TAG);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
DisplaySystem& DisplaySystem::get() {
    static DisplaySystem instance{};
    return instance;
}

// -----------------------------------------------------------------------------
// Event Handling
// -----------------------------------------------------------------------------

void DisplaySystem::processEvent(const SDL_Event& event) {
    spdlog::debug("{} received event type: {}", TAG, event.type);
}
