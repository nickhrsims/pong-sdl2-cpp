#include "core/rendering_system.h"

#include "direct_render_component.h"

DirectRenderComponent::DirectRenderComponent(Entity* entity) : entity{entity} {
    color = {255, 255, 255, 255};
}

DirectRenderComponent::DirectRenderComponent(Entity* entity, SDL_Color& color)
    : entity{entity}, color{color} {}

void DirectRenderComponent::draw() {
    SDL_Rect rect{entity->getRect()};
    RenderingSystem::get().drawRect(rect, color);
}
