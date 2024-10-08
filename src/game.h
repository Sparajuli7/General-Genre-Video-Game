#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "unit.h"
#include "resources.h"
#include <SDL2/SDL.h>
#include <lua.hpp>

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
    Resources* resources;
    lua_State* L;

    std::string getAIAction(Unit* unit);
};

#endif
