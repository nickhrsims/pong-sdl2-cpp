#pragma once

#include "game/components/direct_render_component.h"
#include "game/components/input_component.h"
#include "game/entity.h"
#include "game/player.h"

class Paddle : public Entity {
  public:
    Paddle(Player player);

    void update(float delta) override;
    void draw() const override;

  private:
    InputComponent inputComponent;
    const DirectRenderComponent renderComponent;
};
