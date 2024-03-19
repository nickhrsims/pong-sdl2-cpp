# Decision 0006 - Abstract SDL2 Events

## Motivation

This decision has been updated post-implementation-testing of a global event bus.

Ultimately, the event bus is not likely to bring and benefit except preventing
anything except the `Game` object from utilizing custom events.

Use of events for "general communication" quickly showed awkwardness. The event
bus will probably be a YAGNI situation soon.

## History

### Version 2 (Overturned)

The event system has seen more use throughout the program, and opportunities to
simplify other abstractions are available should the SDL2 event bus concept be
itself abstracted further. The current SDL2 event system is very "C-like" in
nature (as SDL2 itself is a C/C++ library). The previous decision was to "Not
wrap SDL2 events", meaning not to create a C++ struct or class that somehow
abstracts details about how events are stored. This decision does not
necessarily change that reasoning; instead, it describes motivations to
abstract the entire "event bus" concept, while not wasting effort on changing
the nature of the event bus itself (the prevention of which was the goal of the
decision version 1, and is still upheld here).

The "event bus" concept will be abstracted similarly to how the "rendering" and
"input" concepts were. A global event bus will be made available as a core
application sub-system for the sole-purpose of registering events and
publishing events. Core event consumption is still to be exclusively the
responsibility of the sole `App` object. This ensures tight control over
dispatch, and demonstrates a clear path to dispatch. The alternative to this is
allowing subscriptions to occur directly through the event bus, but I fear this
will lead to poor discipline and "spaghettify" event handling, since everything
related to SDL2 events would be routed through such a monstrosity. Unacceptable.

Preventing subscriptions at the event bus level ensures tighter control without
sacrificing flexibility. On the contrary, to makes it clear that events MUST
flow down specific paths; reifying domain-event hierarchies (rather than a flat
mess of criss-crossed subscriptions). This explicit hierarchy then requires that
subscribers go through the proper domain authority:

Examples

    Input events can only be subscribed to from the input bus, and the
    hierarchy makes it so that such a subscription is only possible BECAUSE the
    input bus controls such events and their dispatch, but only receives them
    because it itself receives them as dispatch from the global `App` object.
    This prevents other objects from attempting to process RAW input events,
    only themselves able to subscribe to "pre-processed" abstract notifications
    from the event bus.

    Game-state transition events can only be subscribed to from the Game object,
    and the hierarchy makes it so that a subscription is only possible BECAUSE
    the Game object itself registered the event, and dispatches it internally.

    etc. etc.

### Version 1 (Overturned)

- Decision :: Do not wrap SDL2 Events

- Reason ::
  SDL2 is an integral part of the underpinning systems, what might be called the
  "engine" (however small). It is very likely that an SDL2 wrapper would look
  much like the existing SDL_Event union as it is now.
