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

    void start(const Config& config);
    void stop();

    virtual void processFrame(float delta)      = 0;
    virtual void processEvent(SDL_Event& event) = 0;

  protected:
    App();

  private:
    bool isRunning;
};
