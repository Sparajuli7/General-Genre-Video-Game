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

    static City& makeCity();
    static const std::map<Uuid, City> &getCities() { return cities; };

private:
    MapTile* tile; // Placeholder for map position
    bool unitCreatedThisTurn; // Flag to track if a unit was created this turn
    const Uuid uuid;

    City();
    City(Uuid uuid, MapTile* tile);
    static inline std::map<Uuid, City> cities = std::map<Uuid, City>();
};

#endif