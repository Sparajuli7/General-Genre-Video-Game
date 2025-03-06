#ifndef CITY_H
#define CITY_H

#include <SDL2/SDL.h>
#include <map>

class Unit;

class MapTile;
class Map;

class City {
public:
    // Constructor to initialize the city with an ID and its position on the map
    City(int id, MapTile* tile);

    // Get the ID of the city
    int getID() const;
    int getTileID() const;

    // Check if the city can create a unit this turn
    bool unitCreatedThisTurn; // Flag to track if a unit was created this turn
    bool canCreateUnit() const;

    // Create a unit if allowed
    Unit *createUnit(std::map<int, Unit*> units, int health, int attack);

    // Reset the turn status for unit creation
    void resetTurn();

private:
    int id; // City ID indicating team ownership (1xx for team 1, 2xx for team 2)
    MapTile* tile; 
    
};

#endif