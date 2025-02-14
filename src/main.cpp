#include "game.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    // Initialize random number generation
    srand(time(NULL));

    Game game;
    game.init();

    while (game.isRunning()) {
        game.handleEvents();
        game.update();
        game.render();
        SDL_Delay(100);
    }

    game.clean();
    return 0;
}
