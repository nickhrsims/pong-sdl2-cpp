#pragma once

#include <string>

#include <SDL_events.h>
#include <SDL_scancode.h>

class InputSystem {
  public:
    struct Config {
        struct ActionMap {
            SDL_Scancode playerOneUp;
            SDL_Scancode playerOneDown;
            SDL_Scancode playerTwoUp;
            SDL_Scancode playerTwoDown;
        };
    };

    enum class Action { moveUp, moveDown, confirm, cancel, pause, quit };

    ~InputSystem();

    static InputSystem& get();

    void initialize(const Config& config);
    void terminate();

    void handleEvent(const SDL_Event& event);

    void mapKeyboardKeyToAction(SDL_Scancode key);
    void mapMouseButtonToAction();

    bool isActionPressed(Action action);

  private:
    InputSystem();
    InputSystem(const InputSystem&)             = delete;
    InputSystem(const InputSystem&&)            = delete;
    InputSystem& operator=(const InputSystem&)  = delete;
    InputSystem& operator=(const InputSystem&&) = delete;
};
