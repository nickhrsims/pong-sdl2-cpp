#pragma once

#include "game/entity.h"
#include "game/player.h"

// TODO: Tests
class Paddle : public Entity {
  public:
    Paddle(Player player);

    void update(float delta) override;
    void draw() const override;

  private:
    Player player;
};
