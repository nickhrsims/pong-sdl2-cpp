#include <string>

#include "texture.h"

// -----------------------------------------------------------------------------
// Destructor, Constructors, and Operators
// -----------------------------------------------------------------------------

Texture::~Texture() { SDL_DestroyTexture(data); }

Texture::Texture(SDL_Texture* rawSdlTexture) : data{rawSdlTexture} {
    SDL_QueryTexture(rawSdlTexture, NULL, NULL, &w, &h);
}

Texture::Texture(Texture&& other) : data{other.data}, w{other.w}, h{other.h} {
    other.data = nullptr;
}

Texture& Texture::operator=(Texture&& other) {
    if (data) {
        SDL_DestroyTexture(data);
    }
    data       = other.data;
    w          = other.w;
    h          = other.h;
    other.data = nullptr;
    return *this;
}
