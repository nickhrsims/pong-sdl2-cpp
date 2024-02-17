#pragma once

#include "display_system.h"

class RenderingSystem {
  public:
    struct Config {};

    ~RenderingSystem();

    static RenderingSystem& get();

    void initialize(const Config& config);
    void terminate();

  private:
    RenderingSystem();
    RenderingSystem(const RenderingSystem&)            = delete;
    RenderingSystem(const RenderingSystem&&)           = delete;
    RenderingSystem& operator=(const RenderingSystem&) = delete;
};
