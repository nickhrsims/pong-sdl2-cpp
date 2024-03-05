#include <string>

#include "texture.h"

Texture::Texture(SDL_Texture* rawSdlTexture) : data{rawSdlTexture} {
    SDL_QueryTexture(rawSdlTexture, NULL, NULL, &w, &h);
}
Texture::~Texture() { SDL_DestroyTexture(data); }
