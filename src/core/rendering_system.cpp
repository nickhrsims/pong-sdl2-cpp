#include <spdlog/spdlog.h>

#include "rendering_system.h"

static const std::string TAG{"Rendering Sub-system"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

RenderingSystem::RenderingSystem() {}
RenderingSystem::~RenderingSystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

// FIXME: Not Implemented
void RenderingSystem::initialize(const Config& config) {
    (void)config;
    spdlog::info("Initializing {}.", TAG);
}

// FIXME: Not Implemented
void RenderingSystem::terminate() { spdlog::info("Terminating {}.", TAG); }

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
RenderingSystem& RenderingSystem::get() {
    static RenderingSystem instance{};
    return instance;
}
