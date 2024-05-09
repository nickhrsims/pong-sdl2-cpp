#include "core/color.h"
#include "core/renderer.h"
#include "core/vector2.h"
#include "game/input_bus.h"

#include "paddle.h"

static int const SPEED{400};

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
Paddle::Paddle(Player player) : player{player} { setSize(8, 64); }

// -----------------------------------------------------------------------------
// Static Function Components
// -----------------------------------------------------------------------------
static int getInputDifference(InputBus::Action a, InputBus::Action b) {
    static auto const& input{InputBus::get()};
    return input.isActionPressed(b) - input.isActionPressed(a);
}

static void renderWhiteRect(Rect const& rect) {
    static auto const& renderer{Renderer::get()};
    renderer.drawRect(rect, Color::white());
}

// -----------------------------------------------------------------------------
// Entity Overrides
// -----------------------------------------------------------------------------
void Paddle::update(float delta) {
    int const vx = 0;
    int vy       = 0;
    switch (player) {
    case Player::one:
        vy = getInputDifference(InputBus::Action::playerOneUp,
                                InputBus::Action::playerOneDown);
        break;
    case Player::two:
        vy = getInputDifference(InputBus::Action::playerTwoUp,
                                InputBus::Action::playerTwoDown);
        break;
    }
    setVelocity(vx, vy * SPEED);
    move(delta);
}
void Paddle::draw() const { renderWhiteRect(getRect()); }
