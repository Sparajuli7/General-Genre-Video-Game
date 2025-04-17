#ifndef CITY_H
#define CITY_H

#include <SDL2/SDL.h>
#include <map>
#include "map.h"
#include "unit.h"
//#include "player.h"

class Player;

class City {
public:

    static void renderAll(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);

    // Get the ID of the city
    int getUUID() const { return uuid; } // Getter for id
    int getTileUUID() const { return tile->getUUID(); }

    // Check if the city can create a unit this turn
    bool unitCreatedThisTurn; // Flag to track if a unit was created this turn
    bool canCreateUnit() const;

    // Create a unit if allowed
    static bool createUnit(int cityUUID, int health, int attack, Player* currentPlayer);

    // Reset the turn status for unit creation
    void resetTurn();

    static City* makeCity(MapTile* tile, Player* owner);
  
    static const std::map<int, City*> &getCities() { return cities; };

    Player* getOwner() const { return owner; };
    void setOwner(Player* player) { owner = player; };

private:
    const Uuid uuid;
    City(MapTile* tile, Player* owner);
    MapTile* tile;
    static inline std::map<int, City*> cities = std::map<int, City*>();
    Player* owner;  // add to private section

};

#endif