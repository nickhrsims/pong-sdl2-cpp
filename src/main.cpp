#include "game/game.h"

int main(void) {

    Game game{};

    game.start({
        .assetSystemConfig{},
        .displaySystemConfig{},
        .renderingSystemConfig{},
        .textSystemConfig{},
    });

    return 0;
}
