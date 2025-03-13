#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "map.h"
#include "uuid.hpp"

class MapTile;


class Unit {
public:
    void render(SDL_Renderer* renderer);
    bool move(int targetX, int targetY, Map &map);
    void attack(Unit& target);
    int getX() const;
    int getY() const;
    int getHealth() const;
    int getAttack() const;
    bool isAlive() const;

    static Unit& makeUnit();
    static const std::map<int, Unit>& getUnits() { return units; };

private:
    int team;
    MapTile* tile;
    int x, y;
    int health;
    int damage;
    bool hasMoved;
    const Uuid uuid;

    Unit();
    Unit(int x, int y, int health, int attack, int team);
    static inline std::map<int, Unit> units = std::map<int, Unit>();
};

#endif
