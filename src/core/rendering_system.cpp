#include <spdlog/spdlog.h>

#include "core/display_system.h"
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

void RenderingSystem::initialize(const Config& config) {
    (void)config;
    spdlog::info("Initializing {}.", TAG);
    if (!DisplaySystem::get().window) {
        spdlog::error("{} Error: Window required by renderer is null!", TAG);
    }
    renderer = SDL_CreateRenderer(DisplaySystem::get().window, 0, 0);
    if (!renderer) {
        spdlog::error("{} Error: Renderer create failed!", TAG);
        abort();
    }
}

void RenderingSystem::terminate() {
    spdlog::info("Terminating {}.", TAG);
    SDL_DestroyRenderer(renderer);
}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
RenderingSystem& RenderingSystem::getMutable() {
    static RenderingSystem instance{};
    return instance;
}

const RenderingSystem& RenderingSystem::get() { return getMutable(); }

// -----------------------------------------------------------------------------
// Clear + Show
// -----------------------------------------------------------------------------

void RenderingSystem::clear() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}
void RenderingSystem::show() const { SDL_RenderPresent(renderer); }

// -----------------------------------------------------------------------------
// Draw
// -----------------------------------------------------------------------------

void RenderingSystem::drawRect(const SDL_Rect& rect, const SDL_Color& color) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}
