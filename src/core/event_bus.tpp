#pragma once

#include "event_bus.h"

// -----------------------------------------------------------------------------
// Event Push
// -----------------------------------------------------------------------------

// TODO: Support `SDL_Event.code` member

template <typename EventType>
void EventBus::push(EventBus::Domain domain, EventType eventTypeValue) {
    SDL_Event event;
    SDL_zero(event);
    event.user.type = domain.identity() + static_cast<uint32_t>(eventTypeValue);
    SDL_PushEvent(&event);
}

template <typename EventType, typename Data1>
void EventBus::push(EventBus::Domain domain, EventType eventTypeValue, Data1* data1) {
    SDL_Event event;
    SDL_zero(event);
    event.user.type  = domain.identity() + static_cast<uint32_t>(eventTypeValue);
    event.user.data1 = data1;
    SDL_PushEvent(&event);
}

template <typename EventType, typename Data1, typename Data2>
void EventBus::push(EventBus::Domain domain, EventType eventTypeValue, Data1* data1,
                    Data2* data2) {
    SDL_Event event;
    SDL_zero(event);
    event.user.type  = domain.identity() + static_cast<uint32_t>(eventTypeValue);
    event.user.data1 = data1;
    event.user.data2 = data2;
    SDL_PushEvent(&event);
}
