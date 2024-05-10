#include "fading_text.h"

#include "core/color.h"
#include "core/renderer.h"
#include "game/input_bus.h"

// -----------------------------------------------------------------------------
// Destructor / Constructors / Operators
// -----------------------------------------------------------------------------

FadingText::FadingText(const Font& font, const std::string& text, Vector2 position)
    : texture{Renderer::get().loadTexture(font, text, Color::white())} {
    setPosition(position.x, position.y);
}

// -----------------------------------------------------------------------------
// Entity Overrides
// -----------------------------------------------------------------------------

void FadingText::update(float delta) {
    // --- Animation Update
    // Bounce Effect
    if (anim.alpha <= 60) {
        anim.velocity = AnimationData::speed;
    } else if (anim.alpha >= 236) {
        anim.velocity = -AnimationData::speed;
    }
    // Animation Driver
    anim.alpha += anim.velocity * delta;
    texture.setAlpha(anim.alpha);
}

void FadingText::draw() const {
    static const Renderer& renderer{Renderer::get()};
    Vector2 pos{getPosition()};
    renderer.drawTexture(texture, pos.x, pos.y);
}
