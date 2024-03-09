#pragma once

#include <functional>
#include <memory>

#include <SDL_events.h>

#include "display.h"
#include "renderer.h"

/**
 * Core Application class. Subclass to utilize engine functionality.
 *
 * Must implement custom destructor (even if no-op),
 * processEvent, and processFrame.
 *
 * This class may not exist as a singleton for design reasons. Instead,
 * it has what is called the "Single Object Rule". Attempts to create
 * more than one object of this type will violate this rule and abort.
 *
 * This enables other design choices, like avoiding the need for static-storage
 * objects while enjoying single-point-of-control safety for some properties.
 *
 * DO NOT implement this class with singleton support, the purpose of the
 * single object rule is to avoid requiring singletons and ensure no funny
 * violations of encapsulation occur. If something needs access to an App
 * object, it may be a sign that the design is flawed.
 */
class App {
  public:
    /**
     * Start the `App` instance, executing primary frame/event processing loop.
     * \sa App::stop
     */
    void start();

  protected:
    /**
     * Sub-system configuration aggregate.
     */
    struct Config {
        bool headless;
        Display::Config display;
        Renderer::Config renderer;
    };

    virtual ~App();

    // Do not support copy/move/assignment.
    App(App& app)               = delete;
    App(App&& app)              = delete;
    App& operator=(const App&)  = delete;
    App& operator=(const App&&) = delete;

    /**
     * Start the `App` instance, executing primary frame/event processing loop.
     * \sa App::stop
     */
    void start();

    /**
     * Stop a previously started (and still running) `App`.
     * Does nothing if app has not been started (and/or is not running).
     *
     * \sa App::start
     */
    void stop();

    /**
     * Virtual frame processor.
     *
     * Override to describe how to process a single frame.
     */
    virtual void processFrame(const float delta) = 0;

    /**
     * Virtual event processor.
     *
     * Override to describe how to process a single event.
     *
     * Note that only events not handled internally by `App`
     * are dispatched to this handler.
     */
    virtual void processEvent(const SDL_Event& event) = 0;

    App(const Config& config);

  private:
    /**
     * Dispatch events by type to interested sub-systems.
     */
    void dispatchEvent(const SDL_Event& event);

    /** Internal flag used for control-flow. */
    bool isRunning;
};
