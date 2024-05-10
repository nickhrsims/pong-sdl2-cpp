#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/texture.h"
#include "core/vector2.h"
#include "game/entity.h"

/**
 *
 */
class Countdown : public Entity {
  public:
    using SignedTicks          = int64_t;
    using CountType            = unsigned short;
    using TextureContainerType = std::vector<std::shared_ptr<Texture>>;
    using CallbackType =
        std::function<void()>; // could be optimized via. template parameter

    Countdown(CountType startingNumber, SignedTicks interval,
              TextureContainerType const& textures, CallbackType onTimeout,
              Vector2 position);
    void update(const float delta) override;
    void draw() const override;

  private:
    CountType startingCount;
    CountType currentCount;
    SignedTicks currentTicks;
    SignedTicks interval;
    TextureContainerType textures;
    CallbackType timeoutCallback;
};
