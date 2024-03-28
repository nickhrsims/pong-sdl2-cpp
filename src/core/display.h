#pragma once

#include <string>

#include <SDL_events.h>

// TODO: Tests
// TODO: Docs
class Display {
    friend class App;

  public:
    friend class Renderer;

    struct Config {
        std::string windowTitle;
        unsigned int windowPositionX;
        unsigned int windowPositionY;
        unsigned int windowWidth;
        unsigned int windowHeight;
    };

    ~Display();

    static const Display& get();

    void initialize(const Config& config);
    void terminate();

    void processEvent(const SDL_Event& event);

  private:
    SDL_Window* window;

    static Display& getMutable();

    Display();
    Display(Display&)                  = delete;
    Display(Display&&)                 = delete;
    Display& operator=(const Display&) = delete;
};
