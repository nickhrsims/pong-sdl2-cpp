#include "text_system.h"

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

TextSystem::TextSystem() {}
TextSystem::~TextSystem() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

// FIXME: Not Implemented
void TextSystem::initialize(const Config& config) { (void)config; }

// FIXME: Not Implemented
void TextSystem::terminate() {}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
TextSystem& TextSystem::get() {
    static TextSystem instance{};
    return instance;
}
