#pragma once

#include "SDL_rect.h"
#include "SDL_render.h"

#include "display.h"
#include "font.h"
#include "texture.h"

// TODO: Tests
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
    // TODO: Support Vector2 positioning
    void drawTexture(const Texture& texture, int x, int y) const;
    Texture loadTexture(const Font& font, const std::string& text,
                        const SDL_Color& color) const;

  private:
    SDL_Renderer* renderer;

    static Renderer& getMutable();

    Renderer();
    Renderer(const Renderer&)             = delete;
    Renderer(const Renderer&&)            = delete;
    Renderer& operator=(const Renderer&)  = delete;
    Renderer& operator=(const Renderer&&) = delete;
};
