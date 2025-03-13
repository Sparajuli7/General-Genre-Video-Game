#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <thread>

void gameLogic(Game *game);

int main(int argc, char* argv[]) {
    // Initialize random number generation
    srand(time(NULL));

    Game game;
    game.init();

    // Game logic is in a different thread, while rendering is on this one.
    std::thread thread_obj(gameLogic, &game);
    while (game.isRunning()) {
        game.render();
        SDL_Delay(100);
    }

    thread_obj.detach();
    game.clean();
    return 0;
}

void gameLogic(Game *game){
    while (game->isRunning()) {
        game->handleEvents();
        game->update();
        SDL_Delay(100);
    }
}
