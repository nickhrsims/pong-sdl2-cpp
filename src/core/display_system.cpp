#include <string>

#include <spdlog/spdlog.h>

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

// FIXME: Not Implemented
void DisplaySystem::initialize(const Config& config) {
    (void)config;
    spdlog::info("Initializing {}.", TAG);
}

// FIXME: Not Implemented
void DisplaySystem::terminate() { spdlog::info("Terminating {}.", TAG); }

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
DisplaySystem& DisplaySystem::get() {
    static DisplaySystem instance{};
    return instance;
}
