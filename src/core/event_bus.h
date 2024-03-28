#pragma once

#include <cstdint>

#include <SDL_events.h>

/**
 * Global Event Bus.
 *
 * TODO: Tests
 */
class EventBus {
  public:
    // --- Types

    /**
     * Input-domain of event types.
     *
     * Represents an abstract grouping of events via. a concrete domain of
     * valid numerical inputs.
     *
     * TODO: Refactor Domain to be the "Event Authority" for sending / observering
     */
    struct Domain {
      public:
        Domain(uint32_t identity, uint32_t size);

        /** Get the identity of the event domain */
        uint32_t identity();
        uint32_t size();

        uint32_t rawToDomain(uint32_t rawEventType);

        // TODO: Constrain with explicit uint32_t conversion support (C++ concept?)
        template <typename DomainEvent>
        uint32_t domainToRaw(DomainEvent domainEventType);

      private:
        uint32_t myIdentity;
        uint32_t mySize;
    };

    // --- Static Class
    EventBus() = delete;

    // --- Public API
    static Domain registerDomain(uint32_t size);

    template <typename EventType>
    static void push(Domain domain, EventType eventTypeValue);
    template <typename EventType, typename Data1>
    static void push(Domain domain, EventType eventTypeValue, Data1* data1);
    template <typename EventType, typename Data1, typename Data2>
    static void push(Domain domain, EventType eventTypeValue, Data1* data1,
                     Data2* data2);
};

#include "event_bus.tpp"
