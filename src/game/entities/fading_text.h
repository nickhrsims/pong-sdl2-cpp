#pragma once

#include <functional>
#include <unordered_map>

#include "core/font.h"
#include "core/texture.h"
#include "game/entity.h"
#include "game/input_bus.h"

// TODO: Tests
class FadingText : public Entity {
  public:
    // --- Types
    using Action = InputBus::Action;

    // --- Destructor / Constructors / Operators
    FadingText(const Font& font, const std::string& text, Vector2 position);

    FadingText(const FadingText&)            = delete;
    FadingText(FadingText&&)                 = delete;
    FadingText& operator=(const FadingText&) = delete;
    FadingText& operator=(FadingText&&)      = delete;

    // --- Entity Overrides
    void update(float delta) override;
    void draw() const override;

  private:
    // --- Types
    struct AnimationData {
        static const uint16_t speed{301};
        float velocity{speed};
        uint8_t alpha{100};
    };

    // --- Data Members
    Texture texture;

    AnimationData anim;
};
