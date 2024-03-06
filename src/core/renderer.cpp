#include <spdlog/spdlog.h>

#include "core/display.h"
#include "renderer.h"

static const std::string TAG{"Renderer"};

// -----------------------------------------------------------------------------
// No-op Constructor / Destructor
// -----------------------------------------------------------------------------

Renderer::Renderer() {}
Renderer::~Renderer() {}

// -----------------------------------------------------------------------------
// Initialization / Termination
// -----------------------------------------------------------------------------

void Renderer::initialize(const Config& config) {
    (void)config;
    spdlog::info("Initializing {}.", TAG);
    if (!Display::get().window) {
        spdlog::error("{} Error: Window required by renderer is null!", TAG);
    }
    renderer = SDL_CreateRenderer(Display::get().window, 0, 0);
    if (!renderer) {
        spdlog::error("{} Error: Renderer create failed!", TAG);
        abort();
    }
    spdlog::debug("Initialized {} OK!", TAG);
}

void Renderer::terminate() {
    spdlog::info("Terminating {}.", TAG);
    SDL_DestroyRenderer(renderer);
}

// -----------------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------------

// Return singleton.
Renderer& Renderer::getMutable() {
    static Renderer instance{};
    return instance;
}

const Renderer& Renderer::get() { return getMutable(); }

// -----------------------------------------------------------------------------
// Clear + Show
// -----------------------------------------------------------------------------

void Renderer::clear() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}
void Renderer::show() const { SDL_RenderPresent(renderer); }

// -----------------------------------------------------------------------------
// Draw
// -----------------------------------------------------------------------------

void Renderer::drawRect(const SDL_Rect& rect, const SDL_Color& color) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}
