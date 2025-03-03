#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "unit.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <lua.hpp>
#include <string>
#include <iostream>
#include <deque>
#include <sstream>

class Game {
public:
    Game();
    ~Game();
    void init();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool validate_move(std::vector<std::string> command);
    void listMap();
    bool isRunning() const { return running; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    enum ImmediateCommands{
        none = 0,
        done = 1,
        undo = 2,
        listmap = 3,
        listunit = 4,
        listcity = 5,
        help = 6
    };
    ImmediateCommands convertToImmediate(std::string command);

    enum GameCommands{
        unknown = 0,
        move = 1,
        attack = 2,
        makeunit = 3,
    };
    GameCommands convertToGame(std::string command);

    Map* map;
    
    std::deque<std::string> actions;
    lua_State* L;

    int round;
    int numPlayers;
    std::vector<Player> players;

    std::string getAIAction(Unit* unit);


};

#endif
