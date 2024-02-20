#pragma once

#include "SDL_rect.h"
#include "SDL_render.h"

#include "display_system.h"

class RenderingSystem {
    friend class App;

  public:
    struct Config {};

    ~RenderingSystem();

    static const RenderingSystem& get();

    void initialize(const Config& config);
    void terminate();

    void clear() const;
    void show() const;
    void drawRect(const SDL_Rect& rect, const SDL_Color& color) const;

  private:
    SDL_Renderer* renderer;

    static RenderingSystem& getMutable();

    RenderingSystem();
    RenderingSystem(const RenderingSystem&)             = delete;
    RenderingSystem(const RenderingSystem&&)            = delete;
    RenderingSystem& operator=(const RenderingSystem&)  = delete;
    RenderingSystem& operator=(const RenderingSystem&&) = delete;
};
