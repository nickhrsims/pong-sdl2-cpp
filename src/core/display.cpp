#include <string>

#include <spdlog/spdlog.h>

#include "SDL.h"

#include "display.h"

static const std::string TAG{"Display Sub-system"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

Display::Display() {}
Display::~Display() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

void Display::initialize(const Config& config) {
    spdlog::info("Initializing {}.", TAG);
    // TODO: support flags (fullscreen etc.)
    window = SDL_CreateWindow(config.windowTitle.c_str(), config.windowPositionX,
                              config.windowPositionY, config.windowWidth,
                              config.windowHeight, 0);
    if (!window) {
        spdlog::error("{} Error: Failed to create window!");
    }
}

void Display::terminate() {
    spdlog::info("Terminating {}.", TAG);
    SDL_DestroyWindow(window);
}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
Display& Display::getMutable() {
    static Display instance{};
    return instance;
}

const Display& Display::get() { return getMutable(); }

// -----------------------------------------------------------------------------
// Event Handling
// -----------------------------------------------------------------------------

void Display::processEvent(const SDL_Event& event) { (void)event; }
