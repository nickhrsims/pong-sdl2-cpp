# Decision 0009 - User Interface Elements are Entities

## Motivation

I struggled with this for some time, going so far as to consider writing an
SDL2 widget library compatible with the game's specific rendering opinions to
solve this. Ultimately this view of UI elements was unnecessarily cumbersome.

At first, UI elements as entities felt awkward, as though they just weren't
given a creative enough name or special abstraction. After finding inspiration
from the Source Engine (Valve's game engine), I realized that everything can be
an entity, and that doing otherwise when inconvenient is like trying to turn a
screw with a hammer. Sometimes the tool you have _is_ the right one.
