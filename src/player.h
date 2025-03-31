#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <map>
#include "lua.hpp"
#include "unit.h"
#include "city.h"
#include "map.h"

class Player {
public:
    void render(SDL_Renderer* renderer);
    void takeAction();
    std::vector<Unit*> getUnits();
    std::vector<City*> getCities();

    static Player* makePlayer();
    static const std::map<Uuid, Player*> &getPlayers() { return players; };

private:
    Player();
    lua_State* L;
    bool isBot;
    MapTile* cursor;
    std::vector<Unit*> units;
    std::vector<City*> cities;
    const Uuid uuid;

    static inline std::map<Uuid, Player*> players = std::map<Uuid, Player*>();
};

#endif