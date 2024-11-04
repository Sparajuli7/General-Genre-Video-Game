#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "unit.h"
#include <SDL2/SDL.h>
#include <lua.hpp>
#include <string>

class Game {
public:
    Game();
    ~Game();
    void init();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool isRunning() const { return running; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Map* map;
    Unit* playerUnit;
    Unit* enemyUnit;
    lua_State* L;

    std::string getAIAction(Unit* unit);
};

#endif
