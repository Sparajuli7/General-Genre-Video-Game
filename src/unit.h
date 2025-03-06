#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "map.h"

class MapTile;


class Unit {
public:
    Unit(int id, int health, int attack, MapTile* tile);
    int id;
    void render(SDL_Renderer* renderer);
    bool move(int targetX, int targetY, Map &map);
    void attack(Unit& target);
    int getHealth() const;
    int getAttack() const;
    bool isAlive() const;

private:
    MapTile* tile;
    int health;
    int damage;
    bool hasMoved;
};

#endif
