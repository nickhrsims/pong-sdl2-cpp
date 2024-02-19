#pragma once

#include <string>

#include <SDL_events.h>

class DisplaySystem {
  public:
    struct Config {
        std::string windowTitle;
        unsigned int windowPositionX;
        unsigned int windowPositionY;
        unsigned int windowWidth;
        unsigned int windowHeight;
    };

    ~DisplaySystem();

    static DisplaySystem& get();

    void initialize(const Config& config);
    void terminate();

    void processEvent(const SDL_Event& event);

  private:
    DisplaySystem();
    DisplaySystem(DisplaySystem&)                  = delete;
    DisplaySystem(DisplaySystem&&)                 = delete;
    DisplaySystem& operator=(const DisplaySystem&) = delete;
};
