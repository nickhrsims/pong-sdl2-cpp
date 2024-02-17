#include "rendering_system.h"

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

RenderingSystem::RenderingSystem() {}
RenderingSystem::~RenderingSystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

// FIXME: Not Implemented
void RenderingSystem::initialize(const Config& config) { (void)config; }

// FIXME: Not Implemented
void RenderingSystem::terminate() {}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
RenderingSystem& RenderingSystem::get() {
    static RenderingSystem instance{};
    return instance;
}
