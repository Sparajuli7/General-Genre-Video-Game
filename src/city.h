#ifndef CITY_H
#define CITY_H

#include <SDL2/SDL.h>
#include <map>
#include "map.h"
#include "unit.h"

/*
class Unit;

class MapTile;
class Map;
*/
class City {
public:
    // Constructor to initialize the city with an ID and its position on the map
    City(MapTile* tile);

    static void renderAll(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);

    // Get the ID of the city
    int getUUID() const { return uuid; } // Getter for id
    int getTileUUID() const { return tile->uuid; }

    // Check if the city can create a unit this turn
    bool unitCreatedThisTurn; // Flag to track if a unit was created this turn
    bool canCreateUnit() const;

    // Create a unit if allowed
    Unit *createUnit(int health, int attack);

    // Reset the turn status for unit creation
    void resetTurn();

    static City* makeCity(MapTile* tile);
    static std::map<int, City*> &getCities() { return cities; };

private:
    const Uuid uuid; // City ID indicating team ownership (1xx for team 1, 2xx for team 2)
    MapTile* tile; 

    static inline std::map<int, City*> cities = std::map<int, City*>();
};

#endif