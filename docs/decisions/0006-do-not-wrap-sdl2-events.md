# Decision 0006 - Do not wrap SDL2 events

## Reason

SDL2 is an integral part of the underpinning systems, what might be called the
"engine" (however small). It is very likely that an SDL2 wrapper would look
much like the existing SDL_Event union as it is now.
