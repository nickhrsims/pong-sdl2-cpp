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
      currentState{&startState}, font{"res/font.ttf", 16} {

    // ---------------------------------
    // Event Management
    // ---------------------------------

    // Register events and store the identity.
    identityEvent = SDL_RegisterEvents((int)EventType::lastEvent);

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
        ball.setVelocity(0, 0);

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
    startState.enter = []() {}; // always unused
    startState.exit  = [this]() { spdlog::debug("Leaving {} state", startState.tag); };
    startState.processFrame = [this](const float delta) {
        // --- Renderer
        static const Renderer& render{Renderer::get()};

        // --- State Actors
        // Static
        static unsigned short const speed{301};
        static unsigned char alpha{100};
        static float alphaDirection{speed};
        // Local
        Vector2 fieldCenter{field.getCenter()};
        Texture& texture{stringTextures.at(Game::StringKey::startGame)};

        // --- Animation Update
        // Bounce Effect
        if (alpha <= 60) {
            alphaDirection = speed;
        } else if (alpha >= 236) {
            alphaDirection = -speed;
        }
        // Animation Driver
        alpha += alphaDirection * delta;

        texture.setAlpha(alpha);

        // --- Rendering
        render.clear();

        render.drawTexture(stringTextures.at(Game::StringKey::startGame), fieldCenter.x,
                           fieldCenter.y);

        render.show();
    };
    startState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Reset
    resetState.enter = [this]() {
        leftScore  = 0;
        rightScore = 0;
        next();
    };
    resetState.exit         = []() {};
    resetState.processFrame = [](const float delta) { (void)delta; };
    resetState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Countdown
    // TODO: Provided a context object mechanism

    countdownState.enter = [this]() {
        spdlog::debug("Entering {} state", countdownState.tag);
    };
    countdownState.exit = [this]() {
        spdlog::debug("Leaving {} state", countdownState.tag);
    };
    countdownState.processFrame = [this](const float delta) {
        // --- Ignore unused delta
        (void)delta;
        // --- Renderer
        static const Renderer& renderer{Renderer::get()};
        // --- State Actors
        // Static
        static bool isActive                = false;
        static unsigned long long prevTicks = 0;
        static unsigned long long currTicks = 0;
        static char counter                 = 0;
        static std::vector<Texture*> textures{{
            &stringTextures.at(StringKey::go),
            &numberTextures.at(1),
            &numberTextures.at(2),
            &numberTextures.at(3),
        }};
        // Local
        Vector2 fieldCenter{field.getCenter()};
        // --- Auxiliary Functors
        static auto setup = []() {
            isActive  = true;
            prevTicks = SDL_GetTicks64();
            counter   = 3;
        };
        static auto teardown = [this]() {
            isActive = false;
            next();
        };
        static auto isTimerHit = []() { return currTicks - prevTicks >= 600; };
        static auto onTimerHit = []() {
            --counter;
            prevTicks = currTicks;
        };

        // --- Animation Update
        currTicks = SDL_GetTicks64();

        // --- If just starting (again), do setup
        if (!isActive) {
            setup();
        }

        // --- If delay timer interval hit
        if (isTimerHit()) {
            // If counter is done, teardown and end
            if (counter == 0) {
                teardown();
                return;
            }

            // Handle normal timer-hit operator
            onTimerHit();
        }

        // --- Rendering
        renderer.clear();

        // Draw paddles for "visual effect"
        leftPaddle.draw();
        rightPaddle.draw();

        // NOTE: fix it so the ball doesn't render just before this.
        renderer.drawTexture(*textures[counter], fieldCenter.x, fieldCenter.y);

        //        draw_scores(app->video);
        //      draw_entities(app->video, 2, (entity_t* [2]){&left_paddle,
        //      &right_paddle});
        //    draw_dimmer(app->video);
        //  video_draw_text_with_color(app->video, map[counter], field.x + (field.w
        //  / 2),
        //                           field.y + (field.h / 2), 255, 255, 255, 240);
        renderer.show();
    };
    countdownState.processEvent = [](const SDL_Event& event) { (void)event; };

    // --- Field Setup
    fieldSetupState.enter = [this]() {
        spdlog::debug("Entered {} state", fieldSetupState.tag);
        Vector2 fieldCenter{field.getCenter()};
        ball.setPosition(fieldCenter.x, fieldCenter.y);
        ball.randomizeVelocity();
        next();
    };
    fieldSetupState.exit = [this]() {
        spdlog::debug("Leaving {} state", fieldSetupState.tag);
    };
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
    // Primary switch for system events.
    switch (event.type) {
    case SDL_KEYDOWN:
        InputBus::get().handleKeyDownEvent(event.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        InputBus::get().handleMouseButtonDownEvent(event.button);
        break;
    default:
        // Nested switch for user events.
        switch (event.type - identityEvent) {
        case static_cast<uint32_t>(EventType::transitionRequestEvent):
            handleTransition(static_cast<State*>(event.user.data1));
            break;
        default:
            currentState->processEvent(event);
            break;
        }
        break;
    }
}

// -----------------------------------------------------------------------------
// Trigger-to-State Dispatch
// -----------------------------------------------------------------------------

// TODO: Schedule these rather than doing them immediately
//
// NOTE: Possible solution strategy
// - Refactor this into `handleTransition`, and implement `scheduleTransition`
// - the pushes a custom SDL2 Event onto the global event queue. Let the event
// - propogate down into the `Game` event processor, then dispatch to
// - `handleTransition` which receives exiting and entering states as arguments
// - passed from the event data.
void Game::scheduleTransition(State* target) {
    SDL_Event event;
    SDL_zero(event);
    event.user.type =
        identityEvent + static_cast<uint32_t>(EventType::transitionRequestEvent);
    event.user.data1 = target;
    SDL_PushEvent(&event);
}

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
    ++leftScore;
    next();
}

void Game::handleRightGoal() {
    ++rightScore;
    next();
}
