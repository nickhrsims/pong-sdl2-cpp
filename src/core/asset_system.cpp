#include <spdlog/spdlog.h>

#include "asset_system.h"

static const std::string TAG{"Asset Management Sub-system"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

AssetSystem::AssetSystem() {}
AssetSystem::~AssetSystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

// FIXME: Not Implemented
void AssetSystem::initialize(const Config& config) {
    (void)config;

    spdlog::info("Initializing {}.", TAG);
}

// FIXME: Not Implemented
void AssetSystem::terminate() { spdlog::info("Terminating {}.", TAG); }

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
AssetSystem& AssetSystem::get() {
    static AssetSystem instance{};
    return instance;
}
