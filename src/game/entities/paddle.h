#pragma once

#include "game/components/direct_render_component.h"
#include "game/components/input_component.h"
#include "game/entity.h"

class Paddle : public Entity {
  public:
    Paddle();

    void update(float delta) override;
    void draw() const override;

  private:
    const InputComponent inputComponent;
    const DirectRenderComponent renderComponent;
};
