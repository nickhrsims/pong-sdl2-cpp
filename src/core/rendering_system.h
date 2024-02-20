#pragma once

#include "SDL_rect.h"
#include "SDL_render.h"

#include "display_system.h"

class RenderingSystem {
  public:
    struct Config {};

    ~RenderingSystem();

    static RenderingSystem& get();

    void initialize(const Config& config);
    void terminate();

    void clear();
    void show();
    void drawRect(SDL_Rect& rect, SDL_Color& color);

  private:
    SDL_Renderer* renderer;
    RenderingSystem();
    RenderingSystem(const RenderingSystem&)             = delete;
    RenderingSystem(const RenderingSystem&&)            = delete;
    RenderingSystem& operator=(const RenderingSystem&)  = delete;
    RenderingSystem& operator=(const RenderingSystem&&) = delete;
};
