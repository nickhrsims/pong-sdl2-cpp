#pragma once

#include <string>

#include "core/app.h"
#include "entities/ball.h"
#include "entities/paddle.h"
#include "input_bus.h"

/**
 * A fancy FSM to dispatch `App` control to `Game::State`s.
 */
class Game : public App {
    enum class EventType {
        firstEvent, ///< Do not use.
        transitionRequestEvent,
        lastEvent, ///< Do not use.
    };

    struct State {
        State(const std::string tag) : tag{tag} {};
        const std::string tag;

        std::function<void(const float)> processFrame;
        std::function<void(const SDL_Event&)> processEvent;

        std::function<void()> enter;
        std::function<void()> exit;

        // --- Event-Transition Mappings
        // (These pointers do not "own" their targets)
        State* onDone{nullptr};
        State* onQuit{nullptr};
        State* onPause{nullptr};
        State* onNext{nullptr};
        State* onConfirm{nullptr};
        State* onCancel{nullptr};
        State* onGameOver{nullptr};
    };

  public:
    Game(const App::Config& config);
    ~Game() override;

    Game(Game& game)              = delete;
    Game(Game&& game)             = delete;
    Game& operator=(const Game&)  = delete;
    Game& operator=(const Game&&) = delete;

    void processFrame(const float delta) override;
    void processEvent(const SDL_Event& event) override;

  private:
    // --- Types
    /**
     * Symbolic reference support for associating game-specific
     * strings and textures.
     */
    enum class StringKey { startGame, gameOver, go };

    // --- Data Members
    Rect field;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;
    State* currentState;
    unsigned char leftScore{0};
    unsigned char rightScore{0};
    Font font;

    std::unordered_map<StringKey, std::string> stringValues;
    std::unordered_map<StringKey, Texture> stringTextures;
    std::vector<Texture> numberTextures;

    // --- Rules (Collision, Goal, Score, etc.)
    void handleLeftGoal();
    void handleRightGoal();
    void resolveFrameCollisions();

    // --- Input
    // Various input-action event subscriptions
    InputBus::Subscription actionSubscription;

    // --- State Management
    // States
    State startState{"Start"};
    State resetState{"Reset"};
    State countdownState{"Countdown"};
    State fieldSetupState{"Field Setup"};
    State playingState{"Playing"};
    State pauseState{"Pause"};
    State gameOverState{"Game Over"};
    State shutdownState{"Shutdown"};

    // Triggers
    // "Raw" transition handler
    void handleTransition(State* target);
    // "Dispatch" transition handler
    void scheduleTransition(State* target);
    // Specific transition dispatch
    void done();
    void quit();
    void pause();
    void next();
    void confirm();
    void cancel();
    void gameOver();

    // --- Event Management
    uint32_t identityEvent{0};
};
