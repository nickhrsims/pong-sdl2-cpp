# Decision 0005 - Game-State transition triggers are functions

# Possible Alternatives

Many alternatives for FSM implementations exist. Functions feel
uncomfortable in theory, but in practice as straightforward, simple, and
elegant. My concern is that it is somehow less-scalable compared to a
transition table mapping events to states. It comes down to what is easier
to refactor.
