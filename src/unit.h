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
    bool move(int targetX, int targetY, Map &map);
    void attackUnit(Uuid attackerUUID, Uuid targetUUID);
    
    int getHealth() const;
    int getAttack() const;
    bool isAlive() const;

    static Unit& makeUnit(Uuid uuid, int health, int attackRatio, MapTile* tile);
    static std::map<int, Unit*>& getUnits() { return units; };

private:
    MapTile* tile;
    int health;
    int damage;
    bool hasMoved;
    const Uuid uuid;

    void attack(Unit& target);

    Unit();
    Unit(Uuid uuid, int health, int attack, MapTile* tile);
    static inline std::map<int, Unit*> units = std::map<int, Unit*>();
};

#endif
