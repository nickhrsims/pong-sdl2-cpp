#pragma once

#include "SDL_rect.h"
#include "SDL_render.h"

#include "display.h"

class Renderer {
    friend class App;

  public:
    struct Config {};

    ~Renderer();

    static const Renderer& get();

    void initialize(const Config& config);
    void terminate();

    void clear() const;
    void show() const;
    void drawRect(const SDL_Rect& rect, const SDL_Color& color) const;

  private:
    SDL_Renderer* renderer;

    static Renderer& getMutable();

    Renderer();
    Renderer(const Renderer&)             = delete;
    Renderer(const Renderer&&)            = delete;
    Renderer& operator=(const Renderer&)  = delete;
    Renderer& operator=(const Renderer&&) = delete;
};
