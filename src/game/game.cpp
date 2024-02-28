#include <cassert>

#include <spdlog/spdlog.h>

#include "SDL_scancode.h"

#include "game.h"
#include "game/collision_system.h"
#include "game/entities/paddle.h"
#include "game/input_system.h"

// NOTE: A little bit of pointer chasing is OK for a game this small

// -----------------------------------------------------------------------------
// Constructor / Destructor
// -----------------------------------------------------------------------------

Game::Game(const App::Config& config)
    : App{config},
      field{
          0,
          0,
          static_cast<int>(config.displaySystemConfig.windowWidth),
          static_cast<int>(config.displaySystemConfig.windowHeight),
      },
      leftPaddle{Player::one}, rightPaddle{Player::two}, ball{},
      currentState{&startState} {

    // ---------------------------------
    // Entities
    // ---------------------------------

    // Ball in the center
    const Vector2 fieldCenter{field.getCenter()};
    ball.setPosition(fieldCenter.x, fieldCenter.y);
    // HACK:
    ball.setVelocity(300, 60);

    // How close are the paddles to the walls?
    // (for actual meaning, see `Rect::getVerticalSlice()`)
    static const unsigned int fieldSliceRatio{6};
    // What side of the field?
    static const unsigned int leftFieldIndex{0};
    static const unsigned int rightFieldIndex{fieldSliceRatio - 1};
    // Calculate the center of the left and right "sections" of the field.
    const Vector2 leftFieldSectionCenter{
        field.getVerticalSlice(fieldSliceRatio, leftFieldIndex).getCenter()};
    const Vector2 rightFieldSectionCenter{
        field.getVerticalSlice(fieldSliceRatio, rightFieldIndex).getCenter()};

    // Left Player Paddle (player 1)
    leftPaddle.setPosition(leftFieldSectionCenter.x, leftFieldSectionCenter.y);

    // Right Player Paddle (player 2)
    rightPaddle.setPosition(rightFieldSectionCenter.x, rightFieldSectionCenter.y);

    // ---------------------------------
    // Sub-system Intitialization
    // ---------------------------------

    // --- Input System
    {
        InputSystem& input{InputSystem::get()};
        InputSystem::Config config;
        config.setKeyboardKeyDownAction(SDL_SCANCODE_A,
                                        InputSystem::Action::playerOneUp);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_Z,
                                        InputSystem::Action::playerOneDown);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_K,
                                        InputSystem::Action::playerTwoUp);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_M,
                                        InputSystem::Action::playerTwoDown);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_RETURN,
                                        InputSystem::Action::confirm);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_BACKSPACE,
                                        InputSystem::Action::cancel);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_Q, InputSystem::Action::quit);

        input.initialize(config);

        actionSubscription = input.onActionPressed([this](InputSystem::Action action) {
            switch (action) {
            case InputSystem::Action::quit:
                stop();
                return;
            case InputSystem::Action::pause:
                pause();
                return;
            case InputSystem::Action::confirm:
                confirm();
                return;
            default:
                return;
            }
        });
    }

    // ---------------------------------
    // State Transitions
    // ---------------------------------

    // --- Start
    startState.onConfirm = &fieldSetupState;
    startState.onQuit    = &shutdownState;

    // --- Reset
    resetState.onNext = &fieldSetupState;
    resetState.onQuit = &shutdownState;

    // --- Field Setup
    fieldSetupState.onNext = &countdownState;
    fieldSetupState.onQuit = &shutdownState;

    // --- Countdown
    countdownState.onNext = &playingState;
    countdownState.onQuit = &shutdownState;

    // --- Playing
    playingState.onPause    = &pauseState;
    playingState.onNext     = &fieldSetupState;
    playingState.onGameOver = &gameOverState;
    playingState.onQuit     = &shutdownState;

    // --- Pause
    pauseState.onPause = &playingState;
    pauseState.onQuit  = &shutdownState;

    // --- Game Over
    gameOverState.onConfirm = &resetState;
    gameOverState.onCancel  = &shutdownState;
    gameOverState.onQuit    = &shutdownState;

    // ---------------------------------
    // State Execution Controls
    // ---------------------------------

    // --- Start
    startState.enter        = []() {};
    startState.exit         = []() {};
    startState.processFrame = [](const float delta) { (void)delta; };
    startState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Reset
    resetState.enter        = []() {};
    resetState.exit         = []() {};
    resetState.processFrame = [](const float delta) { (void)delta; };
    resetState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Countdown
    countdownState.enter        = []() {};
    countdownState.exit         = []() {};
    countdownState.processFrame = [](const float delta) { (void)delta; };
    countdownState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Field Setup
    fieldSetupState.enter = []() {

    };
    fieldSetupState.exit         = []() {};
    fieldSetupState.processFrame = [](const float delta) { (void)delta; };
    fieldSetupState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Playing
    playingState.enter        = []() {};
    playingState.exit         = []() {};
    playingState.processFrame = [](const float delta) { (void)delta; };
    playingState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Pause
    pauseState.enter        = []() {};
    pauseState.exit         = []() {};
    pauseState.processFrame = [](const float delta) { (void)delta; };
    pauseState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Game Over
    gameOverState.enter        = []() {};
    gameOverState.exit         = []() {};
    gameOverState.processFrame = [](const float delta) { (void)delta; };
    gameOverState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Shut Down
    shutdownState.enter        = [this]() { stop(); };
    shutdownState.exit         = []() {};
    shutdownState.processFrame = [](const float delta) { (void)delta; };
    shutdownState.processEvent = [](const SDL_Event& event) { (void)event; };

    // ---------------------------------
    // State Member Assertions (debug)
    // ---------------------------------
    // Just check to make sure state->processFrame and state->processEvent
    // are both defined, these are just pseudo-assertions to make sure I
    // didn't forget to define anything, they should be removed in release
    // builds.
#ifndef NDEBUG
    State* debugStateAssertionChecklist[]{
        &startState, &resetState,     &fieldSetupState, &playingState,
        &pauseState, &countdownState, &shutdownState,   &gameOverState};

    bool error = false;

    for (State* state : debugStateAssertionChecklist) {
        if ((error |= !state->processFrame)) {
            spdlog::error("{} state does not define a frame processor!", state->tag);
        }

        if ((error |= !state->processEvent)) {
            spdlog::error("{} state does not define an event processor!", state->tag);
        }
    }

    if (error) {
        abort();
    }

#endif
}
Game::~Game() { InputSystem::get().offActionPressed(actionSubscription); }

// -----------------------------------------------------------------------------
// Frame / Event Processing Dispatch
// -----------------------------------------------------------------------------

inline void Game::processFrame(const float delta) { currentState->processFrame(delta); }
inline void Game::processEvent(const SDL_Event& event) {
    switch (event.type) {

    case SDL_KEYDOWN:
        InputSystem::get().handleKeyDownEvent(event.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        InputSystem::get().handleMouseButtonDownEvent(event.button);
        break;

    default:
        currentState->processEvent(event);
        break;
    }
}

// -----------------------------------------------------------------------------
// Trigger-to-State Dispatch
// -----------------------------------------------------------------------------

void Game::transition(State* next) {
    // If `next` is defined (a transition exists for the proved event)
    // - see specific event callers for example.
    if (next) {
        // Call the exit handler of the old state, if the handler exists
        if (currentState->exit) {
            currentState->exit();
        }
        // Transition to the next state provided by the caller
        currentState = next;
        // Call the enter handler of the new state, if the handler exists
        if (currentState->enter) {
            currentState->enter();
        }
    }
}

void Game::done() { transition(currentState->onDone); }
void Game::quit() { transition(currentState->onQuit); }
void Game::pause() { transition(currentState->onPause); }
void Game::next() { transition(currentState->onNext); }
void Game::confirm() { transition(currentState->onConfirm); }
void Game::cancel() { transition(currentState->onCancel); }
void Game::gameOver() { transition(currentState->onGameOver); }
