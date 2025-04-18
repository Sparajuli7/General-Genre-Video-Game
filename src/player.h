#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <map>
#include <algorithm>
#include "lua.hpp"
//#include "unit.h"
//#include "city.h"
#include "map.h"
#include "uuid.hpp"

class Unit;
class City;
//class MapTile;


class Player {
public:
    void render(SDL_Renderer* renderer);
    void takeAction();
    std::vector<Unit*> getUnits();
    std::vector<City*> getCities();

    static Player* makePlayer();
    static const std::map<Uuid, Player*> &getPlayers() { return players; };

    const Uuid getUUID() { return uuid; };

    void addCity(City* city) { cities.push_back(city); }
    void addUnit(Unit* unit) { units.push_back(unit); };

    const void removeCity(City* city) {
        Player::cities.erase(std::remove(cities.begin(), cities.end(), city), cities.end());
    }

    const void removeUnit(Unit* unit) {
        Player::units.erase(std::remove(units.begin(), units.end(), unit), units.end());
    }

    bool isDefeated() const { return cities.empty(); }



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