#pragma once

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include <SDL_events.h>
#include <SDL_scancode.h>

// TODO: Separate input bus from game-specific actions
// NOTE: Consider pub-sub with message broker (event channel)
// TODO: Tests
class InputBus {
  public:
    // ---------------------------------
    // Supported Player Actions & Types
    // ---------------------------------

    /**
     * Input Types.
     */
    enum class InputType {
        none,
        keyboard,
        mouse,
    };

    /**
     * Input Actions.
     */
    enum class Action {
        none,
        playerOneUp,
        playerOneDown,
        playerTwoUp,
        playerTwoDown,
        confirm,
        cancel,
        pause,
        quit
    };

    // ---------------------------------
    // Subscription Node
    // ---------------------------------

    // TODO: Wrap iterator in RAII to auto-unsubscribe on out-of-scope,
    //       or use custom delete over std::unique_ptr template.
    typedef std::_List_iterator<std::function<void(InputBus::Action)>> Subscription;

    // ---------------------------------
    // Configuration
    // ---------------------------------

    // TODO: Separate action map by player.

    struct Config {
        friend InputBus;

        // It is undefined behavior to assign multiple inputs to the same action.
        // TODO: Consider supporting multiple inputs / input-types per action.

        /**
         * Relate a keyboard key scancode to an action.
         *
         * This will cause the action to be triggered when the key is pressed.
         */
        void setKeyboardKeyDownAction(SDL_Scancode scancode, Action action);

        /**
         * Relate a mouse button to an action.
         *
         * This will cause the action to be triggered when the button is pressed.
         */
        void setMouseButtonDownAction(uint8_t button, Action action);

      private:
        struct KeyboardKeyActionDescription {
            KeyboardKeyActionDescription(){}; // by default do nothing interesting.
            KeyboardKeyActionDescription(SDL_Scancode scancode);
            InputType type;
            SDL_Scancode scancode;
        };

        struct MouseButtonActionDescription {
            MouseButtonActionDescription(){}; // by default do nothing interesting.
            MouseButtonActionDescription(uint8_t button);
            InputType type;
            uint8_t button;
        };

        union ActionDescription {
            ActionDescription(){};
            InputType type;
            KeyboardKeyActionDescription keyboard;
            MouseButtonActionDescription mouse;
        };

        std::unordered_map<Action, ActionDescription> actionMap;
    };

    ~InputBus();

    static InputBus& get();

    void initialize(const Config& config);
    void terminate();
    void reinitialize(const Config& config);

    void handleKeyDownEvent(const SDL_KeyboardEvent& event) const;
    void handleMouseButtonDownEvent(const SDL_MouseButtonEvent& event) const;

    bool isActionPressed(Action action) const;
    Subscription onActionPressed(std::function<void(Action action)> callback);
    void offActionPressed(Subscription subscription);

    SDL_Event x;

  private:
    // --- Utility Action Queires

    bool isKeyboardKeyDownActionPressed(Action action) const;
    bool isMouseButtonDownActionPressed(Action action) const;

    // --- Action Maps

    /**
     * Relate actions to their input type.
     */
    std::unordered_map<Action, InputType> actionToInputTypeMap;

    /**
     * Relates actions to scancodes.
     */
    std::unordered_map<Action, SDL_Scancode> actionToScancodeMap;

    /**
     * Relates actions to mouse buttons.
     */
    std::unordered_map<Action, uint8_t> actionToMouseButtonMap;

    /**
     * Relates scancodes to actions.
     */
    Action scancodeToActionMap[SDL_NUM_SCANCODES];
    /**
     *Relates mouse buttons to actions.
     *
     * SDL2 implementation-specific.
     * SDL_mouse.h provides 5 macro-definitions ranging 1..5 inclusive.
     * This means that the map must have a length of 6 to support all
     * mouse indices (without needing to subject the mouse button, which
     * would require an explanation everywhere it happens, and extra byte
     * isn't much when compared to making sense of code).
     */
    static const uint8_t mouseButtonCount{6};
    Action buttonToActionMap[mouseButtonCount]; // Read note above on magic number `6`

    // --- Observers

    std::list<std::function<void(Action action)>> observers;

    // --- Explicitely deleted constructors

    InputBus();
    InputBus(const InputBus&)             = delete;
    InputBus(const InputBus&&)            = delete;
    InputBus& operator=(const InputBus&)  = delete;
    InputBus& operator=(const InputBus&&) = delete;
};
