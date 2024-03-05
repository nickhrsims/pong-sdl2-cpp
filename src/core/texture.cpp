#include <string>

#include "texture.h"

Texture::Texture(SDL_Texture* rawSdlTexture) : data{rawSdlTexture} {}
Texture::~Texture() { SDL_DestroyTexture(data); }
