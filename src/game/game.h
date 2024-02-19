#include <string>

#include "core/app.h"

/**
 * A fancy FSM to dispatch `App` control to `Game::State`s.
 */
class Game : public App {
  public:
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

    friend State;

    Game(const App::Config& config);
    ~Game() override;

    Game(Game& game)              = delete;
    Game(Game&& game)             = delete;
    Game& operator=(const Game&)  = delete;
    Game& operator=(const Game&&) = delete;

    void processFrame(const float delta) override;
    void processEvent(const SDL_Event& event) override;

  private:
    State* currentState;

    // States;
    State startState{"Start"};
    State resetState{"Reset"};
    State countdownState{"Countdown"};
    State fieldSetupState{"Field Setup"};
    State playingState{"Playing"};
    State pauseState{"Pause"};
    State gameOverState{"Game Over"};
    State shutdownState{"Shutdown"};

    // --- Triggers
    // "Raw" transition handler
    void transition(State* next);
    /// Specific transition dispatch
    std::function<void()> done{[this]() { transition(currentState->onDone); }};
    std::function<void()> quit{[this]() { transition(currentState->onQuit); }};
    std::function<void()> pause{[this]() { transition(currentState->onPause); }};
    std::function<void()> next{[this]() { transition(currentState->onNext); }};
    std::function<void()> confirm{[this]() { transition(currentState->onConfirm); }};
    std::function<void()> cancel{[this]() { transition(currentState->onCancel); }};
    std::function<void()> gameOver{[this]() { transition(currentState->onGameOver); }};
};
