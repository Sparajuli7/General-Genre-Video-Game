#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "map.h"
#include "uuid.hpp"

class City;
class Player;

class Unit {
public:

    /* Rendering functions */
    static void renderAll(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    
    /* Unit Stats and Info */
    int getHealth() const { return health; }
    int getAttack() const { return damage; }
    bool isAlive() const { return health > 0; }
    bool hasMoved() const { return moved; }
    bool moved; // Could be private, but its easier to change while public.
    
    /* Owner functions */
    Player* getOwner() const { return owner; };
    void setOwner(Player* player) { owner = player; };

    /* Tile and UUID functions */
    int getUUID() const { return uuid; } 
    int getTileUUID() const { return tile->uuid; } 
    MapTile* getTile() const { return tile; } // Getter for unit's current tile

    /* Static functions that make use of units map */

    // Create a Unit and place UUID and pointer into units map
    static Unit* makeUnit(int health, int attackRatio, MapTile* tile, Player* owner);
    
    // Getter function to access units map
    static std::map<int, Unit*>& getUnits() { return units; };
    
    // Get unit based on UUID
    static Unit* uuidToUnit(Uuid uuid) { return units.at(uuid); };
    
    // Move and attack actions
    static bool move(int movingUUID, int targetTileId, Map& map, Player* currentPlayer);
    static bool attackUnit(int attackerUUID, int targetUUID, Player* currentPlayer);
    
    

private:

    /* Init functions, units are intended to be created using the makeUnit function */
    Unit();
    Unit(int health, int attack, MapTile* tile, Player* owner);

    /* Stats and info */
    int health;
    int damage;
    const Uuid uuid;
    Player *owner;
    MapTile* tile;

    // Simple function that will remove health from target unit based on unit's attack
    void attack(Unit& target) { target.health -= damage; }

    // Map function that contains a unit pointer and its UUID
    static inline std::map<int, Unit*> units = std::map<int, Unit*>();
};

#endif
