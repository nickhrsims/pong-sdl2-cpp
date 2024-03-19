#include "main_menu.h"

#include "core/color.h"
#include "core/renderer.h"
#include "game/input_bus.h"

// -----------------------------------------------------------------------------
// Destructor / Constructors / Operators
// -----------------------------------------------------------------------------

MainMenu::~MainMenu() {
    InputBus& input{InputBus::get()};
    input.offActionPressed(actionSubscription);
}

MainMenu::MainMenu(const Params params)
    : texture{Renderer::get().loadTexture(params.font, "PRESS START", Color::white())},
      handleConfirm{params.onConfirm} {

    InputBus& input{InputBus::get()};

    actionSubscription = input.onActionPressed([this](Action action) {
        switch (action) {
        case Action::confirm:
            handleConfirm();
            break;
        default:
            break;
        }
    });
}

// -----------------------------------------------------------------------------
// Entity Overrides
// -----------------------------------------------------------------------------

void MainMenu::update(float delta) {
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

void MainMenu::draw() const {
    static const Renderer& renderer{Renderer::get()};
    Vector2 pos{getPosition()};
    renderer.drawTexture(texture, pos.x, pos.y);
}

// -----------------------------------------------------------------------------
// Callback Injector
// -----------------------------------------------------------------------------

void MainMenu::onConfirm(Callback callback) { handleConfirm = callback; }
