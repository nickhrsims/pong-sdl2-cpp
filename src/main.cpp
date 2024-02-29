#include "spdlog/common.h"
#include <spdlog/spdlog.h>

#include "game/game.h"

int main(void) {

    spdlog::set_level(spdlog::level::debug);

    Game game{{
        .display{
            .windowTitle     = "Pong SDL2 C++",
            .windowPositionX = 256,
            .windowPositionY = 256,
            .windowWidth     = 256,
            .windowHeight    = 256,
        },
        .renderer{},
    }};

    game.start();

    return 0;
}
