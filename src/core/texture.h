#pragma once

#include <SDL.h>

#include "font.h"

struct Texture {
    // --- Constructors, Destructor, Operators
    ~Texture();
    Texture(SDL_Texture* rawSdlTexture);
    Texture(Texture&& other);
    Texture& operator=(Texture&& rhs);
    // --- Disable copy
    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    // --- Members
    SDL_Texture* data;
    int w;
    int h;
};
