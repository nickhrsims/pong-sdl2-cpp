#include "core/app.h"

class Game : public App {
  public:
    Game();
    ~Game() override;

    Game(Game& game)  = delete;
    Game(Game&& game) = delete;

    void processFrame(float delta) override;
    void processEvent(SDL_Event& event) override;
};
