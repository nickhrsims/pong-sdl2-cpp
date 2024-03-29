#include "core/renderer.h"

#include "direct_render_component.h"

DirectRenderComponent::DirectRenderComponent(Entity* entity) : entity{entity} {
    color = {255, 255, 255, 255};
}

DirectRenderComponent::DirectRenderComponent(Entity* entity, SDL_Color& color)
    : entity{entity}, color{color} {}

void DirectRenderComponent::draw() const {
    Renderer::get().drawRect(entity->getRect(), color);
}
