#pragma once

#include <functional>
#include <memory>

#include <SDL_events.h>

#include "asset_system.h"
#include "display_system.h"
#include "rendering_system.h"
#include "text_system.h"

class App {
  public:
    /**
     * Sub-system configuration aggregate.
     */
    struct Config {
        AssetSystem::Config assetSystemConfig;
        DisplaySystem::Config displaySystemConfig;
        RenderingSystem::Config renderingSystemConfig;
        TextSystem::Config textSystemConfig;
    };

    virtual ~App();

    // Do not support copy/move/assignment.
    App(App& app)              = delete;
    App(App&& app)             = delete;
    App& operator=(const App&) = delete;

    /**
     * Start the `App` instance, executing primary frame/event processing loop.
     * \sa App::stop
     */
    void start(const Config& config);

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
    virtual void processFrame(float delta) = 0;

    /**
     * Virtual event processor.
     *
     * Override to describe how to process a single event.
     */
    virtual void processEvent(SDL_Event& event) = 0;

  protected:
    App();

  private:
    /** Internal flag used for control-flow. */
    bool isRunning;
};
