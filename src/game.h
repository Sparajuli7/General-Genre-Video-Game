#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "city.h"
#include "unit.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <lua.hpp>
#include <random>
#include <string>
#include <iostream>
#include <deque>
#include <sstream>

class Game {
// TODO: A review is needed to ensure functions and variables are correctly set as public or private.
public:
    Game();
    ~Game();
    void init();
    void handleEvents();
    void update();
    void render();
    void clean();
    static void requestInput();

    static bool validate_move(GameCommand command);
    bool isRunning() { return running; }

private:
    // Disable and enable rendering
    bool renderOn = true;

    /* SDL rendering things */
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool running;

    // Enum to represent commands that can be run by a human player.
    enum ImmediateCommands{
        none = 0,
        done = 1,
        undo = 2,
        listmap = 3,
        listunit = 4,
        listcity = 5,
        help = 6,
        quit = 7
    };
    static ImmediateCommands convertToImmediate(std::string command);

    /* Commands that list game information to human player */
    void listMap(); // Unused, could be used to provide a player without GUI map connections
    void listUnit();
    void listCity();

    static GameCommand convertToGame(std::string command);

    /* Variables for current game */
    Map* map;
    int round;
    Player* currentPlayer;
    int numPlayers;
    std::vector<Player> players;

    std::string getAIAction(Unit* unit);

    void checkVictoryCondition();
};

#endif
