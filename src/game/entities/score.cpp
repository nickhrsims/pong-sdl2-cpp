#include <stdexcept>

#include "core/color.h"
#include "core/renderer.h"

#include "score.h"

// -----------------------------------------------------------------------------
// Destructor / Constructors / Operators
// -----------------------------------------------------------------------------

Score::~Score() {}

Score::Score(const Params params)
    : max{params.max}, textures{createIntegerTextureVector(params.font, params.max)} {}

// -----------------------------------------------------------------------------
// Entity Overrides
// -----------------------------------------------------------------------------

void Score::draw() const {
    static const Renderer& renderer{Renderer::get()};
    Vector2 pos{getPosition()};
    renderer.drawTexture(textures[value], pos.x, pos.y);
}

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

// TODO: Send to log
void Score::increment() {
    if (isAtMax()) {
        throw std::out_of_range(
            std::string("Cannot increment score further, must be within range [0, ") +
            std::to_string(max) + std::string("]"));
    }
    ++value;
}
void Score::reset() { value = 0; }
bool Score::isAtMax() { return (value == max); }

// -----------------------------------------------------------------------------
// Private Utilities
// -----------------------------------------------------------------------------

std::vector<Texture> Score::createIntegerTextureVector(const Font& font,
                                                       ValueType limit) {
    const Renderer& renderer{Renderer::get()};
    const SDL_Color white{Color::white()};
    std::vector<Texture> textures;
    textures.reserve(limit);
    // Technically allow for limit "plus one"
    for (auto idx = 0; idx <= limit; ++idx) {
        textures.emplace_back(renderer.loadTexture(font, std::to_string(idx), white));
    }
    return textures;
}
