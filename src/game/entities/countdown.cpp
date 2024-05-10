#include "countdown.h"
#include "core/renderer.h"

// -----------------------------------------------------------------------------
// Static Function Components
// -----------------------------------------------------------------------------
using SignedTicks = Countdown::SignedTicks;
using Count       = Countdown::CountType;

static unsigned short const MS{1000}; // Milliseconds converstion

// Not the most efficient to run division every frame,
// but probably faster than all the branching and relational checks
// that were happening in the last algorithm; including calls
// into SDL2 for grabbing tick count directly.
static Count getCountFromTicks(SignedTicks ticks, SignedTicks interval) {
    Count count = ticks / interval; // unsigned overflow possible, but unsigned
                                    // required for logical safety

    return static_cast<Count>(ticks >= 0 ? count : 0); // Do not return if
                                                       // ticks (signed) is
                                                       // negative, which
                                                       // implies overflow of
                                                       // count.
}

static SignedTicks getResetTicks(Count startingCount, SignedTicks interval) {
    return (startingCount + 1) * interval;
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
Countdown::Countdown(CountType startingCount, SignedTicks interval,
                     TextureContainerType const& textures, CallbackType onTimeout,
                     Vector2 position)
    : startingCount{startingCount},
      currentTicks{getResetTicks(startingCount, interval)}, interval{interval},
      textures{textures}, timeoutCallback{onTimeout} {
    if (textures.size() < startingCount) {
        throw std::length_error("given texture vector is smaller than required "
                                "by given starting count!");
    }
    setPosition(position.x, position.y);
}

// -----------------------------------------------------------------------------
// Entity Overrides
// -----------------------------------------------------------------------------

void Countdown::update(float const delta) {
    currentTicks -= (delta * MS);
    if (currentTicks < 0) {
        currentTicks = getResetTicks(startingCount, interval);
        timeoutCallback();
    } else {
        currentCount = getCountFromTicks(currentTicks, interval);
    }
}

void Countdown::draw() const {
    static auto const& renderer{Renderer::get()};
    auto const pos{getPosition()};
    renderer.drawTexture(*textures[currentCount], pos.x, pos.y);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------
