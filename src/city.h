#ifndef CITY_H
#define CITY_H

#include <SDL2/SDL.h>
#include <map>
#include "map.h"
#include "unit.h"

class Player;

class City {
public:

    /* Rendering functions*/
    static void renderAll(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);

    int getUUID() const { return uuid; } 
    int getTileUUID() const { return tile->uuid; }
    Player* getOwner() const { return owner; };
    void setOwner(Player* player) { owner = player; };

    /* Variables and functions related to unit creation */
    static bool createUnit(int cityUUID, int health, int attack, Player* currentPlayer);
    bool unitCreatedThisTurn; // Flag to track if a unit was created this turn
    bool canCreateUnit() const { return !unitCreatedThisTurn; };
    void resetTurn() { unitCreatedThisTurn = false; }; 

    /* Static functions that make use of the cities map*/
    static City* makeCity(MapTile* tile, Player* owner);
    static const std::map<int, City*> &getCities() { return cities; };
    static City* uuidToCity(Uuid uuid) { return cities.at(uuid); };


private:
    
    City(MapTile* tile, Player* owner);
    static inline std::map<int, City*> cities = std::map<int, City*>();
    const Uuid uuid;
    Player* owner;  
    MapTile* tile;
    
};

#endif