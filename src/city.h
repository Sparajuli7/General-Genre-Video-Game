#ifndef CITY_H
#define CITY_H

#include <SDL2/SDL.h>
#include <map>
#include "map.h"
#include "unit.h"

class City {
public:

    static void renderAll(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);

    // Get the ID of the city
    int getUUID() const { return uuid; } // Getter for id
    int getTileUUID() const { return tile->uuid; }

    // Check if the city can create a unit this turn
    bool unitCreatedThisTurn; // Flag to track if a unit was created this turn
    bool canCreateUnit() const;

    // Create a unit if allowed
    static bool createUnit(int cityUUID, int health, int attack);

    // Reset the turn status for unit creation
    void resetTurn();

    static City* makeCity(MapTile* tile);
  
    static const std::map<int, City*> &getCities() { return cities; };

private:
    const Uuid uuid;
    City(MapTile* tile);
    MapTile* tile;
    static inline std::map<int, City*> cities = std::map<int, City*>();
};

#endif