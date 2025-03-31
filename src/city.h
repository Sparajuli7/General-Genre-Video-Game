#ifndef CITY_H
#define CITY_H

#include <SDL2/SDL.h>
#include <map>
#include "map.h"
#include "unit.h"

class City {
public:
    // Get the ID of the city
    int getID() const;
    int getTileID() const;

    // Check if the city can create a unit this turn
    bool unitCreatedThisTurn; // Flag to track if a unit was created this turn
    bool canCreateUnit() const;

    // Create a unit if allowed
    Unit *createUnit(int health, int attack);

    // Reset the turn status for unit creation
    void resetTurn();

    static City* makeCity(MapTile* tile);
    static const std::map<Uuid, City*> &getCities() { return cities; };

private:
    const Uuid uuid;
    City(MapTile* tile);
    ~City();
    MapTile* tile;

    static inline std::map<Uuid, City*> cities = std::map<Uuid, City*>();
};

#endif