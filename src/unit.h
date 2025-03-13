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
    void render(SDL_Renderer* renderer);
    bool move(int targetX, int targetY, Map &map);
    void attack(Unit& target);
    int getHealth() const;
    int getAttack() const;
    bool isAlive() const;

    static Unit& makeUnit();
    static const std::map<Uuid, Unit>& getUnits() { return units; };

private:
    MapTile* tile;
    int health;
    int damage;
    bool hasMoved;
    const Uuid uuid;

    Unit();
    Unit(int x, int y, int health, int attack, int team);
    static inline std::map<Uuid, Unit> units = std::map<Uuid, Unit>();
};

#endif
