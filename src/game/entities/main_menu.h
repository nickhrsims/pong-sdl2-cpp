#pragma once

#include <functional>
#include <unordered_map>

#include "core/font.h"
#include "core/texture.h"
#include "game/entity.h"
#include "game/input_bus.h"

class MainMenu : public Entity {
  public:
    // --- Types
    using Action = InputBus::Action;
    typedef std::function<void()> Callback;

    struct Params {
        const Font& font;
        Callback onConfirm{nullptr};
    };

    // --- Destructor / Constructors / Operators
    ~MainMenu();
    MainMenu(const Params);

    MainMenu(const MainMenu&)            = delete;
    MainMenu(MainMenu&&)                 = delete;
    MainMenu& operator=(const MainMenu&) = delete;
    MainMenu& operator=(MainMenu&&)      = delete;

    // --- Entity Overrides
    void update(float delta) override;
    void draw() const override;

    // --- Callback Injector
    void onConfirm(Callback callback);

  private:
    // --- Types
    struct AnimationData {
        static const uint16_t speed{301};
        float velocity{speed};
        uint8_t alpha{100};
    };

    // --- Data Members
    Texture texture;
    Callback handleConfirm;
    InputBus::Subscription actionSubscription;

    AnimationData anim;
};
