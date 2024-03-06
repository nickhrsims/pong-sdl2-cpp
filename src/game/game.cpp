#include <cassert>

#include <spdlog/spdlog.h>

#include "SDL_scancode.h"

#include "game.h"
#include "game/entities/paddle.h"
#include "game/input_bus.h"

// NOTE: A little bit of pointer chasing is OK for a game this small

// -----------------------------------------------------------------------------
// Constructor / Destructor
// -----------------------------------------------------------------------------

Game::Game(const App::Config& config)
    : App{config},
      field{
          0,
          0,
          static_cast<int>(config.display.windowWidth),
          static_cast<int>(config.display.windowHeight),
      },
      leftPaddle{Player::one}, rightPaddle{Player::two}, ball{},
      currentState{&playingState}, font{"res/font.ttf", 24} {

    // ---------------------------------
    // Strings & Textures
    // ---------------------------------
    {
        using Key = StringKey;

        const SDL_Color white{255, 255, 255, 255};

        const Renderer& renderer{Renderer::get()};

        // Initialize Strings
        stringValues[Key::startGame] = "PRESS START";
        stringValues[Key::gameOver]  = "GAME OVER";
        stringValues[Key::go]        = "GO!";

        // Initialize String Text-Textures
        for (auto const& [key, text] : stringValues) {
            stringTextures.try_emplace(key, renderer.loadTexture(font, text, white));
        }

        // Initialize Number Text-Textures
        for (int n = 0; n < 10; ++n) {
            numberTextures.push_back(
                renderer.loadTexture(font, std::to_string(n), white));
        }
    }

    // ---------------------------------
    // Entities
    // ---------------------------------
    {
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
    }

    // ---------------------------------
    // Sub-system Intitialization
    // ---------------------------------

    // --- Input System
    {
        InputBus& input{InputBus::get()};
        InputBus::Config config;
        config.setKeyboardKeyDownAction(SDL_SCANCODE_A, InputBus::Action::playerOneUp);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_Z,
                                        InputBus::Action::playerOneDown);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_K, InputBus::Action::playerTwoUp);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_M,
                                        InputBus::Action::playerTwoDown);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_RETURN, InputBus::Action::confirm);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_BACKSPACE,
                                        InputBus::Action::cancel);
        config.setKeyboardKeyDownAction(SDL_SCANCODE_Q, InputBus::Action::quit);

        input.initialize(config);

        actionSubscription = input.onActionPressed([this](InputBus::Action action) {
            switch (action) {
            case InputBus::Action::quit:
                stop();
                return;
            case InputBus::Action::pause:
                pause();
                return;
            case InputBus::Action::confirm:
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
    startState.enter        = []() {}; // always unused
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
    playingState.processFrame = [this](const float delta) {
        static const Renderer& render{Renderer::get()};

        // --- Update

        ball.update(delta);
        leftPaddle.update(delta);
        rightPaddle.update(delta);

        // --- Collide

        resolveFrameCollisions();

        // --- Render

        render.clear();

        ball.draw();
        leftPaddle.draw();
        rightPaddle.draw();

        render.show();
    };
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
Game::~Game() { InputBus::get().offActionPressed(actionSubscription); }

// -----------------------------------------------------------------------------
// Frame / Event Processing Dispatch
// -----------------------------------------------------------------------------

inline void Game::processFrame(const float delta) { currentState->processFrame(delta); }
inline void Game::processEvent(const SDL_Event& event) {
    switch (event.type) {

    case SDL_KEYDOWN:
        InputBus::get().handleKeyDownEvent(event.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        InputBus::get().handleMouseButtonDownEvent(event.button);
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

// -----------------------------------------------------------------------------
// Rules Processing (Collision, Goals, Score, etc)
// -----------------------------------------------------------------------------

void Game::resolveFrameCollisions() {
    Paddle& lp{leftPaddle};
    Paddle& rp{rightPaddle};
    Ball& b{ball};
    Rect& f{field};

    // --- Left Paddle & Field
    if (lp.getTopEdgePosition() < f.y) {
        lp.setTopEdgePosition(f.y);
    } else if (lp.getBottomEdgePosition() > f.y + f.h) {
        lp.setBottomEdgePosition(f.y + f.h);
    }

    // --- Right Paddle & Field
    if (rp.getTopEdgePosition() < f.y) {
        rp.setTopEdgePosition(f.y);
    } else if (rp.getBottomEdgePosition() > f.y + f.h) {
        rp.setBottomEdgePosition(f.y + f.h);
    }

    // --- Ball & Field
    if (b.getTopEdgePosition() < f.y) {
        Vector2 v{b.getVelocity()};
        b.setVelocity(v.x, std::abs(v.y));
    } else if (b.getBottomEdgePosition() > f.y + f.h) {
        Vector2 v{b.getVelocity()};
        b.setVelocity(v.x, -std::abs(v.y));
    } else if (b.getLeftEdgePosition() < f.x) {
        handleLeftGoal();
    } else if (b.getRightEdgePosition() > f.x + f.w) {
        handleRightGoal();
    }

    // --- Ball & Left Paddle
    if ((lp.getRect() - b.getRect()).hasPoint(0, 0)) {
        Vector2 v{b.getVelocity()};
        b.setVelocity(std::abs(v.x), v.y);
    }

    // --- Ball & Right Paddle
    if ((rp.getRect() - b.getRect()).hasPoint(0, 0)) {
        Vector2 v{b.getVelocity()};
        b.setVelocity(-std::abs(v.x), v.y);
    }
}

void Game::handleLeftGoal() {
    ++leftScore;
    next();
}

void Game::handleRightGoal() {
    ++rightScore;
    next();
}
