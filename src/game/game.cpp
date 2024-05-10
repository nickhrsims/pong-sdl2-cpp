#include <cassert>

#include <spdlog/spdlog.h>

#include "SDL_scancode.h"

#include "core/color.h"
#include "game.h"
#include "game/entities/countdown.h"
#include "game/entities/main_menu.h"
#include "game/entities/paddle.h"

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
      currentState{&startState}, font{"res/font.ttf", 16},
      leftScore{{.font = font, .max = Game::maxScore}},
      rightScore{{.font = font, .max = Game::maxScore}}, mainMenu{{.font = font}} {

    // ---------------------------------
    // Entities
    // ---------------------------------
    {
        const Vector2 ratio{6, 24};
        const Vector2 fieldCenter{field.getCenter()};
        ball.setPosition(fieldCenter.x, fieldCenter.y);
        ball.setVelocity(0, 0);

        // Left Player Paddle (player 1)
        leftPaddle.setPosition(field.w / ratio.x, fieldCenter.y);

        // Right Player Paddle (player 2)
        rightPaddle.setPosition(field.w - (field.w / ratio.x), fieldCenter.y);

        // Left Score
        leftScore.setPosition(fieldCenter.x - (field.w / ratio.x), field.h / ratio.y);

        // Right Score
        rightScore.setPosition(fieldCenter.x + (field.w / ratio.x), field.h / ratio.y);

        // --- Main Menu
        // Set position to center field
        mainMenu.setPosition(fieldCenter.x, fieldCenter.y);
        // Wire MainMenu confirm handler to Game confirm handler.
        mainMenu.onConfirm([this]() { confirm(); });
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
    startState.processFrame = [this](const float delta) {
        // --- Update
        mainMenu.update(delta);

        // --- Rendering
        static const Renderer& renderer{Renderer::get()};
        renderer.clear();

        mainMenu.draw();

        renderer.show();
    };

    // --- Reset
    resetState.enter = [this]() {
        leftScore.reset();
        rightScore.reset();
        next();
    };
    resetState.processFrame = [](const float delta) { (void)delta; };

    // --- Countdown
    // TODO: Move to Countdown entity

    countdownState.processFrame = [this](const float delta) {
        // --- Renderer
        static const Renderer& renderer{Renderer::get()};

        static auto const createNumberTexture = [this](int number) -> Texture {
            return renderer.loadTexture(font, std::to_string(number), Color::white());
        };

        static auto const createStringTexture =
            [this](char const* const text) -> Texture {
            return renderer.loadTexture(font, text, Color::white());
        };

        static Countdown countdown{
            3,
            600,
            {std::make_shared<Texture>(createStringTexture("GO!")),
             std::make_shared<Texture>(createNumberTexture(1)),
             std::make_shared<Texture>(createNumberTexture(2)),
             std::make_shared<Texture>(createNumberTexture(3))},
            [this]() { next(); },
            field.getCenter()};

        countdown.update(delta);

        // --- Rendering
        renderer.clear();

        // Draw paddles for "visual effect"
        leftPaddle.draw();
        rightPaddle.draw();
        leftScore.draw();
        rightScore.draw();
        countdown.draw();

        // NOTE: fix it so the ball doesn't render just before this.

        //        draw_scores(app->video);
        //      draw_entities(app->video, 2, (entity_t* [2]){&left_paddle,
        //      &right_paddle});
        //    draw_dimmer(app->video);
        //  video_draw_text_with_color(app->video, map[counter], field.x + (field.w
        //  / 2),
        //                           field.y + (field.h / 2), 255, 255, 255, 240);
        renderer.show();
    };

    // --- Field Setup
    fieldSetupState.enter = [this]() {
        Vector2 fieldCenter{field.getCenter()};
        ball.setPosition(fieldCenter.x, fieldCenter.y);
        ball.randomizeVelocity();
        next();
    };
    fieldSetupState.processFrame = [](const float delta) { (void)delta; };

    // --- Playing
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
        leftScore.draw();
        rightScore.draw();

        render.show();
    };

    // --- Pause
    pauseState.processFrame = [](const float delta) { (void)delta; };

    // --- Game Over
    gameOverState.enter        = []() {};
    gameOverState.exit         = []() {};
    gameOverState.processFrame = [](const float delta) { (void)delta; };

    // --- Shut Down
    shutdownState.enter        = [this]() { stop(); };
    shutdownState.exit         = []() {};
    shutdownState.processFrame = [](const float delta) { (void)delta; };

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
        bool frameproc_error{false};
        if ((frameproc_error = !state->processFrame)) {
            spdlog::error("{} state does not define a frame processor!", state->tag);
        }

        error |= frameproc_error;
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

void Game::processFrame(const float delta) {
    if (!transitionQueue.empty()) {
        handleTransition(transitionQueue.front());
        transitionQueue.pop();
    }
    currentState->processFrame(delta);
}
void Game::processEvent(const SDL_Event& event) {
    // Primary switch for system events.
    switch (event.type) {
    case SDL_KEYDOWN:
        InputBus::get().handleKeyDownEvent(event.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        InputBus::get().handleMouseButtonDownEvent(event.button);
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Trigger-to-State Dispatch
// -----------------------------------------------------------------------------

// TODO: Generalize Event Bus
void Game::scheduleTransition(State* target) { transitionQueue.push(target); }

void Game::handleTransition(State* target) {
    // If `target` is defined (a transition exists for the proved event)
    // - see specific event callers for example.
    if (target) {
        // Call the exit handler of the old state, if the handler exists
        if (currentState->exit) {
            currentState->exit();
        }
        // Transition to the target state provided by the caller
        currentState = target;
        // Call the enter handler of the new state, if the handler exists
        if (currentState->enter) {
            currentState->enter();
        }
    }
}

void Game::done() { scheduleTransition(currentState->onDone); }
void Game::quit() { scheduleTransition(currentState->onQuit); }
void Game::pause() { scheduleTransition(currentState->onPause); }
void Game::next() { scheduleTransition(currentState->onNext); }
void Game::confirm() { scheduleTransition(currentState->onConfirm); }
void Game::cancel() { scheduleTransition(currentState->onCancel); }
void Game::gameOver() { scheduleTransition(currentState->onGameOver); }

// -----------------------------------------------------------------------------
// Rules Processing (Collision, Goals, Score, etc)
// -----------------------------------------------------------------------------

// TODO: Generalize Physics Processing
// NOTE: Overturn decision 0008.
void Game::resolveFrameCollisions() {
    Paddle& lp{leftPaddle};
    Paddle& rp{rightPaddle};
    Ball& b{ball};
    Rect& f{field};

    // --- Left Paddle & Field
    if (lp.getTopEdgePosition() < f.y) {
        // Align to top of field
        lp.setTopEdgePosition(f.y);
    } else if (lp.getBottomEdgePosition() > f.y + f.h) {
        // Align to bottom of field
        lp.setBottomEdgePosition(f.y + f.h);
    }

    // --- Right Paddle & Field
    if (rp.getTopEdgePosition() < f.y) {
        // Aligned to top of field
        rp.setTopEdgePosition(f.y);
    } else if (rp.getBottomEdgePosition() > f.y + f.h) {
        // Align to bottom of field
        rp.setBottomEdgePosition(f.y + f.h);
    }

    // --- Ball & Field
    if (b.getTopEdgePosition() < f.y) {
        // Bounce
        Vector2 v{b.getVelocity()};
        b.setVelocity(v.x, std::abs(v.y));
    } else if (b.getBottomEdgePosition() > f.y + f.h) {
        // Bounde
        Vector2 v{b.getVelocity()};
        b.setVelocity(v.x, -std::abs(v.y));
    } else if (b.getLeftEdgePosition() < f.x) {
        // Delegate field-goal handler
        handleLeftGoal();
    } else if (b.getRightEdgePosition() > f.x + f.w) {
        // Delegate field-goal handler
        handleRightGoal();
    }

    // --- Ball & Left Paddle
    if ((lp.getRect() - b.getRect()).hasPoint(0, 0)) {
        // Bounce
        Vector2 v{b.getVelocity()};
        b.setVelocity(std::abs(v.x), v.y);
    }

    // --- Ball & Right Paddle
    if ((rp.getRect() - b.getRect()).hasPoint(0, 0)) {
        // Bounce
        Vector2 v{b.getVelocity()};
        b.setVelocity(-std::abs(v.x), v.y);
    }
}

void Game::handleLeftGoal() {
    leftScore.increment();
    next();
}

void Game::handleRightGoal() {
    rightScore.increment();
    next();
}
