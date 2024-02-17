#pragma once

class DisplaySystem {
  public:
    struct Config {
        unsigned int windowPositionX;
        unsigned int windowPositionY;
        unsigned int windowWidth;
        unsigned int windowHeight;
    };

    ~DisplaySystem();

    static DisplaySystem& get();

    void initialize(const Config& config);
    void terminate();

  private:
    DisplaySystem();
    DisplaySystem(DisplaySystem&)                  = delete;
    DisplaySystem(DisplaySystem&&)                 = delete;
    DisplaySystem& operator=(const DisplaySystem&) = delete;
};
