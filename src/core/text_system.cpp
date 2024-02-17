#include <spdlog/spdlog.h>

#include "text_system.h"

static const std::string TAG{"Text Management Sub-system"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

TextSystem::TextSystem() {}
TextSystem::~TextSystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

// FIXME: Not Implemented
void TextSystem::initialize(const Config& config) {
    (void)config;
    spdlog::info("Initializing {}.", TAG);
}

// FIXME: Not Implemented
void TextSystem::terminate() { spdlog::info("Terminating {}.", TAG); }

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
TextSystem& TextSystem::get() {
    static TextSystem instance{};
    return instance;
}
