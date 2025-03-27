#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "map.h"
#include "uuid.hpp"

//class MapTile;
//class Map;


class Unit {
public:
    static void renderAll(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    static bool move(Uuid movingUUID, int targetTileId, Map& map);
    void attackUnit(Uuid attackerUUID, Uuid targetUUID);
    
    int getHealth() const;
    int getAttack() const;
    bool isAlive() const;
    bool hasMoved() const { return moved; }
    void resetTurn(); // To reset hasMoved at the end of a turn


    int getUUID() const { return uuid; } // Getter for id
    int getTileUUID() const { return tile->uuid; }
    MapTile* getTile() const { return tile; } // Getter for current tile

    static Unit* makeUnit(int health, int attackRatio, MapTile* tile);
    static std::map<int, Unit*>& getUnits() { return units; };

private:
    MapTile* tile;
    int health;
    int damage;
    bool moved;
    const Uuid uuid;

    void attack(Unit& target);

    Unit();
    Unit(int health, int attack, MapTile* tile);
    static inline std::map<int, Unit*> units = std::map<int, Unit*>();
};

#endif
