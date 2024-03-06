#include <spdlog/spdlog.h>

#include "display.h"
#include "font.h"
#include "renderer.h"
#include "texture.h"

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

void Renderer::drawTexture(const Texture& texture, int x, int y) const {
    int cx{x - (texture.w / 2)};
    int cy{y - (texture.h / 2)};
    SDL_Rect rect{cx, cy, texture.w, texture.h};
    SDL_RenderCopy(renderer, texture.data, NULL, &rect);
}

// -----------------------------------------------------------------------------
// Resource Management
// -----------------------------------------------------------------------------

Texture Renderer::loadTexture(const Font& font, const std::string& text,
                              const SDL_Color& color) const {
    SDL_Surface* surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);
    Texture texture{SDL_CreateTextureFromSurface(Renderer::get().renderer, surface)};
    SDL_FreeSurface(surface);
    return texture;
}
