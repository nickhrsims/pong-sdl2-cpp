#include "display_system.h"

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

DisplaySystem::DisplaySystem() {}
DisplaySystem::~DisplaySystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

// FIXME: Not Implemented
void DisplaySystem::initialize(const Config& config) { (void)config; }

// FIXME: Not Implemented
void DisplaySystem::terminate() {}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
DisplaySystem& DisplaySystem::get() {
    static DisplaySystem instance{};
    return instance;
}
