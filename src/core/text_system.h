#pragma once

#include "renderer.h"

class TextSystem {
    friend class App;

  public:
    struct Config {};
    ~TextSystem();

    static const TextSystem& get();

    void initialize(const Config& config);
    void terminate();

  private:
    static TextSystem& getMutable();

    TextSystem();
    TextSystem(const TextSystem& rhs)        = delete;
    TextSystem(const TextSystem&& rhs)       = delete;
    TextSystem& operator=(const TextSystem&) = delete;
};
