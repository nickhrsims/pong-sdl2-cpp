#pragma once

#include <string>

#include <SDL_events.h>

class DisplaySystem {
  public:
    friend class RenderingSystem;

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
    SDL_Window* window;

    DisplaySystem();
    DisplaySystem(DisplaySystem&)                  = delete;
    DisplaySystem(DisplaySystem&&)                 = delete;
    DisplaySystem& operator=(const DisplaySystem&) = delete;
};
