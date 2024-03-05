#pragma once

#include <SDL.h>

#include "font.h"

class Texture {
  public:
    Texture(SDL_Texture* rawSdlTexture);
    ~Texture();

  private:
    SDL_Texture* data;
};
