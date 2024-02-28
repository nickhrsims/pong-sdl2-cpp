#pragma once

#include "game/entity.h"
#include "game/input_bus.h"
#include "game/player.h"

class InputComponent {
  public:
    InputComponent(Entity* entity, Player player);

    void update(float delta) const;

  private:
    Entity* const entity;
    InputBus::Action upAction;
    InputBus::Action downAction;
};
;
