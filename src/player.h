#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <map>
#include <algorithm>
#include <deque>
#include "lua.hpp"
#include "map.h"
#include "uuid.hpp"

class Unit;
class City;

// This is hackyyyyy
enum GameCommandId {
    Unknown,
    Move,
    Attack,
    Makeunit
};

struct GameCommand {
    GameCommandId id;
    union {
        struct {
            int unit;
            int mapTile;
        } move;

        struct {
            int unit;
            int target;
        } attack;

        struct {
            int city;
        } makeUnit;
    };
};

class Player {
public:

    std::deque<GameCommand> takeAction();
    
    const Uuid getUUID() { return uuid; }; 

    /* Vectors and functions that handle the player's units and cities */
    std::vector<Unit*> getUnits() { return this->units; };
    std::vector<City*> getCities() { return this->cities; };

    void addCity(City* city) { cities.push_back(city); }
    void addUnit(Unit* unit) { units.push_back(unit); };

    const void removeCity(City* city) {
        Player::cities.erase(std::remove(cities.begin(), cities.end(), city), cities.end());
    }

    const void removeUnit(Unit* unit) {
        Player::units.erase(std::remove(units.begin(), units.end(), unit), units.end());
    }

    // Player is considered to be defeated if they have no more cities
    bool isDefeated() const { return cities.empty(); } 
    
    /* Static functions that make use of the players map */
    static Player* makePlayer(bool isBot);
    static const std::map<Uuid, Player*> &getPlayers() { return players; };
    static Player* uuidToPlayer(Uuid uuid) { return players.at(uuid); };


private:
    Player(bool isBot); // makePlayer is intended to be the public initializer
    
    // AI stuff
    lua_State* L;
    bool isBot; 
    
    std::vector<Unit*> units;
    std::vector<City*> cities;
    const Uuid uuid;

    //MapTile* cursor; // Should be used for UI implementation 

    // Map function that contains a player pointer and its UUID
    static inline std::map<Uuid, Player*> players = std::map<Uuid, Player*>();
};

#endif