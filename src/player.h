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
    std::vector<Unit> getUnits();
    std::vector<City> getCities();

    static Player& makePlayer();
    static const std::map<int, Player> &getPlayers() { return players; };

private:
    lua_State* L;
    bool isBot;
    MapTile* cursor;
    std::vector<Unit> units;
    std::vector<City> cities;
    const Uuid uuid;

    Player();
    static inline std::map<int, Player> players = std::map<int, Player>();
};

#endif