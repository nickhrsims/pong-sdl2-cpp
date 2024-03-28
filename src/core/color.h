#pragma once

#include <SDL_pixels.h>

// TODO: Tests
namespace Color {

const SDL_Color generate(float r, float b, float g, float a);

const SDL_Color white(float alpha);
const SDL_Color white();

const SDL_Color black(float alpha);
const SDL_Color black();

} // namespace Color
