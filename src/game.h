#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "unit.h"
#include <SDL2/SDL.h>
#include <lua.hpp>
#include <string>
#include <iostream>
#include <deque>

class Game {
public:
    Game();
    ~Game();
    void init();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool validate_move(std::string command);
    bool isRunning() const { return running; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Map* map;
    int round;
    std::deque<std::string> actions;

};

#endif
