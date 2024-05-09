#pragma once

#include "game/entity.h"

// TODO: Tests
class Ball : public Entity {
  public:
    Ball();
    void update(float delta) override;
    void draw() const override;

    void randomizeVelocity();

  private:
    double speed{0};
    static const int defaultSpeed{300};
};
