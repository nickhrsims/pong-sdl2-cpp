#include "event_bus.h"

#include <SDL_events.h>

// -----------------------------------------------------------------------------
// Domain Struct
// -----------------------------------------------------------------------------
EventBus::Domain::Domain(uint32_t identity, uint32_t size)
    : myIdentity{identity}, mySize{size} {}

uint32_t EventBus::Domain::identity() { return myIdentity; }

uint32_t EventBus::Domain::size() { return mySize; }

uint32_t EventBus::Domain::rawToDomain(uint32_t rawEventType) {
    return rawEventType - myIdentity;
}

template <typename DomainEvent>
uint32_t EventBus::Domain::domainToRaw(DomainEvent domainEventType) {
    return myIdentity + static_cast<uint32_t>(domainEventType);
}

// -----------------------------------------------------------------------------
// Domain Registration
// -----------------------------------------------------------------------------

EventBus::Domain EventBus::registerDomain(uint32_t size) {
    return Domain{SDL_RegisterEvents(size), size};
}
