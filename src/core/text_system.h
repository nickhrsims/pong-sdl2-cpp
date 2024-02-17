#pragma once

#include "rendering_system.h"

class TextSystem {
  public:
    struct Config {};
    ~TextSystem();

    static TextSystem& get();

    void initialize(const Config& config);
    void terminate();

  private:
    TextSystem();
    TextSystem(const TextSystem& rhs)        = delete;
    TextSystem(const TextSystem&& rhs)       = delete;
    TextSystem& operator=(const TextSystem&) = delete;
};
