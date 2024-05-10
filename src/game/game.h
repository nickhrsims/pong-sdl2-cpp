#pragma once

#include <queue>
#include <string>

#include "core/app.h"
#include "game/entities/ball.h"
#include "game/entities/main_menu.h"
#include "game/entities/paddle.h"
#include "game/entities/score.h"
#include "game/input_bus.h"

/**
 * A fancy FSM to dispatch `App` control to `Game::State`s.
 *
 * TODO: Tests
 * TODO: Separation of States
 */
class Game : public App {
    struct State {
        State(const std::string tag) : tag{tag} {};
        const std::string tag;

        std::function<void(const float)> processFrame;

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
    // --- Data Members
    std::queue<State*> transitionQueue;
    Rect field;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;
    State* currentState;
    Font font;
    Score leftScore;
    Score rightScore;
    MainMenu mainMenu;

    // --- Static Members
    static const Score::ValueType maxScore{6};

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
};
