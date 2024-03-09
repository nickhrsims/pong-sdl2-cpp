#include <csignal>

#include <spdlog/spdlog.h>

#include "core/app.h"
#include "spdlog/common.h"

/**
 * Test Application Type.
 */
struct MyApp : public App {
    MyApp() : App({.headless = true}) {}
    ~MyApp() {}
    void processEvent(const SDL_Event& event) { (void)event; }
    void processFrame(float delta) { (void)delta; }
};

/**
 * Required since abort is raised as part of enforcing the single-object rule.
 */
extern "C" void abortSignalOverride(int signalNumber) {
    (void)signalNumber;
    spdlog::info("Abort signal caught! (If you're seeing this, the test passed)");
    exit(0);
    return;
}

int main() {
    signal(SIGABRT, &abortSignalOverride);

    // Attempt multiple construction
    MyApp app1;
    MyApp app2;

    // Abort should only have been called once!
    return 1;
}
