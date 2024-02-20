#pragma once

#include "game/entity.h"
#include "game/input_system.h"

class InputComponent {
  public:
    InputComponent(Entity* entity);

    void update(float delta) const;

  private:
    Entity* const entity;
};
