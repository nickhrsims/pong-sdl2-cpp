#pragma once

#include "game/components/direct_render_component.h"
#include "game/components/input_component.h"
#include "game/entity.h"
#include "game/player.h"

// TODO: Tests
class Paddle : public Entity {
  public:
    Paddle(Player player);

    void update(float delta) override;
    void draw() const override;

  private:
    // NOTE: There is no reason for components to necessarily be types, nor to be
    //       contrained by the concept "component" borrowed from ECS. This is not
    //       an ECS implementation, and does not benefit from composing "types"
    //       that are just "functions in disguise".
    InputComponent inputComponent;
    const DirectRenderComponent renderComponent;
};
