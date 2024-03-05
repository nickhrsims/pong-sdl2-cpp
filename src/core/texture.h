#pragma once

#include <SDL.h>

#include "font.h"

struct Texture {
    Texture(SDL_Texture* rawSdlTexture);
    ~Texture();
    SDL_Texture* data;
    int w;
    int h;
};
