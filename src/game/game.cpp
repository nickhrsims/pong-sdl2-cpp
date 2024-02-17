#include <spdlog/spdlog.h>

#include "game.h"

// -----------------------------------------------------------------------------
// Constructor / Destructor
// -----------------------------------------------------------------------------

Game::Game() : App{} {}
Game::~Game() {}

// -----------------------------------------------------------------------------
// Frame / Event Processors
// -----------------------------------------------------------------------------

// FIXME: Not implemented.
void Game::processFrame(float delta) {
    (void)delta;

    spdlog::warn("GAME LOOP NOT IMPLEMENTED");

    stop();
}

// FIXME: Not implemented
void Game::processEvent(SDL_Event& event) { (void)event; }
