#pragma once

#include "game/entity.h"
class DirectRenderComponent {
  public:
    DirectRenderComponent(Entity* entity);
    DirectRenderComponent(Entity* entity, SDL_Color& color);

    void draw();

  private:
    Entity* const entity;
    SDL_Color color;
};
