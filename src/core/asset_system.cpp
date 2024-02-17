#include "asset_system.h"

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

AssetSystem::AssetSystem() {}
AssetSystem::~AssetSystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

// FIXME: Not Implemented
void AssetSystem::initialize(const Config& config) { (void)config; }

// FIXME: Not Implemented
void AssetSystem::terminate() {}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
AssetSystem& AssetSystem::get() {
    static AssetSystem instance{};
    return instance;
}
