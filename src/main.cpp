#include "game.h"

int main(int argc, char* argv[]) {
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
