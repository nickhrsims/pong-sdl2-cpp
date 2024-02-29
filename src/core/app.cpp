
#include <SDL.h>
#include <spdlog/spdlog.h>

#include "app.h"
#include "core/renderer.h"

// -----------------------------------------------------------------------------
// No-Op Construction/Destruction
// -----------------------------------------------------------------------------

App::App(const Config& config) {
    // --- Initialize Sub-systems
    Display::getMutable().initialize(config.display);
    Renderer::getMutable().initialize(config.renderer);
}
App::~App() {
    // --- Termiante Sub-systems
    Renderer::getMutable().terminate();
    Display::getMutable().terminate();

    SDL_Quit();
}

// -----------------------------------------------------------------------------
// Start / Stop
// -----------------------------------------------------------------------------

void App::start() {

    spdlog::info("Application started");

    // --- Execute Simulation Loop

    // Set running flag.
    isRunning = true;

    /** CPU ticks at the start of the last frame. */
    uint64_t previousFrameTicks = SDL_GetTicks64();

    /** CPU ticks at the start of this frame. */
    uint64_t currentFrameTicks = 0;

    /** Time at the beginning of the frame. */
    uint64_t frameStartTime = 0;

    /** Time at the end of the frame. */
    uint64_t frameEndTime = 0;

    /** Milliseconds this frame has taken. */
    float elapsedFrameMs = 0;

    /** Time between frames. Measured in seconds. */
    float delta = 0;

    // --- Application Loop
    while (isRunning) {

        // --- Start Frame Timing

        frameStartTime    = SDL_GetPerformanceCounter();
        currentFrameTicks = SDL_GetTicks64();
        delta             = (currentFrameTicks - previousFrameTicks) / 1000.0f;

        // --- Poll input events
        /** Input Event Processing */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            this->dispatchEvent(event);
        }

        // --- Process Frame
        this->processFrame(delta);

        // --- End Frame Timing
        //
        previousFrameTicks = currentFrameTicks;
        frameEndTime       = SDL_GetPerformanceCounter();
        elapsedFrameMs     = (frameEndTime - frameStartTime) /
                         (float)SDL_GetPerformanceFrequency() * 1000.0f;

        // 60 FPS in Milliseconds
        // == 1 (frame) / 60 (seconds) * 1000 (convert to ms)
        static float const FPS60 = 16.666f;

        long frameDelayMs = floor(FPS60 - elapsedFrameMs);

        // Delay each frame to get as close to 60FPS as possible.
        if (frameDelayMs < 0)
            frameDelayMs = 0;
        SDL_Delay(frameDelayMs);
    }

    spdlog::info("Application stopped");
}

void App::stop() {
    spdlog::info("Stopping application...");
    isRunning = false;
}

// -----------------------------------------------------------------------------
// Event Dispatch
// -----------------------------------------------------------------------------

void App::dispatchEvent(const SDL_Event& event) {
    switch (event.type) {
    case SDL_DISPLAYEVENT:
    case SDL_WINDOWEVENT:
        Display::getMutable().processEvent(event);
        break;
    case SDL_QUIT:
        stop();
        break;
    default:
        processEvent(event);
        break;
    }
}
