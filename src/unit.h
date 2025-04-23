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

//class MapTile;
//class Map;


class Unit {
public:
    static void renderAll(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    static bool move(int movingUUID, int targetTileId, Map& map, Player* currentPlayer);
    static bool attackUnit(int attackerUUID, int targetUUID, Player* currentPlayer);
    
    int getHealth() const;
    int getAttack() const;
    bool isAlive() const;
    bool hasMoved() const { return moved; }
    bool moved;
    void resetTurn(); // To reset hasMoved at the end of a turn


    int getUUID() const { return uuid; } // Getter for id
    int getTileUUID() const { return tile->uuid; }
    MapTile* getTile() const { return tile; } // Getter for current tile

    static Unit* makeUnit(int health, int attackRatio, MapTile* tile, Player* owner);
    static std::map<int, Unit*>& getUnits() { return units; };
    static Unit* uuidToUnit(Uuid uuid) { return units.at(uuid); };

    Player* getOwner() const { return owner; };
    void setOwner(Player* player) { owner = player; };

private:
    MapTile* tile;
    int health;
    int damage;
    
    const Uuid uuid;
    Player *owner;

    void attack(Unit& target);

    Unit();
    Unit(int health, int attack, MapTile* tile, Player* owner);
    static inline std::map<int, Unit*> units = std::map<int, Unit*>();
};

#endif
