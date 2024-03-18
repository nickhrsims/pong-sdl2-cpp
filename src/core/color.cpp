#include "color.h"

const SDL_Color Color::generate(float r, float g, float b, float a) {
    // Scaled R, G, B, A
    const uint8_t sR{static_cast<uint8_t>(std::round(r * 255))};
    const uint8_t sG{static_cast<uint8_t>(std::round(g * 255))};
    const uint8_t sB{static_cast<uint8_t>(std::round(b * 255))};
    const uint8_t sA{static_cast<uint8_t>(std::round(a * 255))};
    return SDL_Color{sR, sG, sB, sA};
}

const SDL_Color Color::white(float alpha) { return generate(1.0, 1.0, 1.0, alpha); }

const SDL_Color Color::white() { return white(1.0); }

const SDL_Color Color::black(float alpha) { return generate(0, 0, 0, alpha); }

const SDL_Color Color::black() { return black(1.0); }
