# Decision 0001 - Application Sub-system Late Initialization

# Considered

Initializing as part of constructors

- problems:
  - Removes some elegance in passing configuration and controlling dependency loading order (and injection).
  - All of this must be done in the initializer list of the app constructor, feels a little clunky.
