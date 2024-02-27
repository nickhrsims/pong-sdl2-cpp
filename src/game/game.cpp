#include <cassert>

#include <spdlog/spdlog.h>

#include "SDL_scancode.h"
#include "game.h"
#include "game/entities/paddle.h"
#include "game/input_system.h"

// -----------------------------------------------------------------------------
// Constructor / Destructor
// -----------------------------------------------------------------------------

Game::Game(const App::Config& config)
    : App{config}, currentState{&startState},
      field{
          0,
          0,
          static_cast<int>(config.displaySystemConfig.windowWidth),
          static_cast<int>(config.displaySystemConfig.windowHeight),
      } {

    // ---------------------------------
    // Entities
    // ---------------------------------

    // How close are the paddles to the walls?
    // (for actual meaning, see `Rect::getVerticalSlice()`)
    static const unsigned int fieldSliceRatio{6};
    // What side of the field?
    static const unsigned int leftField{0};
    static const unsigned int rightField{fieldSliceRatio - 1};
    // Calculate the center of the left and right "sections" of the field.
    const Vector2 leftFieldSectionCenter{
        field.getVerticalSlice(fieldSliceRatio, leftField).getCenter()};
    const Vector2 rightFieldSectionCenter{
        field.getVerticalSlice(fieldSliceRatio, rightField).getCenter()};

    // Left Player Paddle (player 1)
    std::unique_ptr<Paddle> leftPaddle{new Paddle{Player::one}};
    leftPaddle->setPosition(leftFieldSectionCenter.x, leftFieldSectionCenter.y);

    // Right Player Paddle (player 2)
    std::unique_ptr<Paddle> rightPaddle(new Paddle{Player::two});
    rightPaddle->setPosition(rightFieldSectionCenter.x, rightFieldSectionCenter.y);

    // We store paddles separate from the ball for various reasons.
    paddles.push_back(std::move(leftPaddle));
    paddles.push_back(std::move(rightPaddle));

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
    startState.enter = [this]() { spdlog::debug("Entering {} state", startState.tag); };
    startState.exit  = []() {};
    startState.processFrame = [this](const float delta) {
        static const RenderingSystem& render{RenderingSystem::get()};

        // --- Update

        for (auto& entity : paddles) {
            entity->update(delta);
        }

        // --- Render

        render.clear();

        for (auto const& entity : paddles) {
            entity->draw();
        }

        render.show();
    };
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
