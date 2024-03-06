#pragma once

#include "game/components/direct_render_component.h"
#include "game/entity.h"

class Ball : public Entity {
  public:
    Ball();
    void update(float delta) override;
    void draw() const override;

    void randomizeVelocity();

  private:
    const DirectRenderComponent renderComponent;
    double speed{0};
    static const int defaultSpeed{300};
};
