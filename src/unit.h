#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "map.h"

class MapTile;


class Unit {
public:
    Unit(int x, int y, int health, int attack, int team);
    void render(SDL_Renderer* renderer);
    bool move(int targetX, int targetY, Map &map);
    void attack(Unit& target);
    int getX() const;
    int getY() const;
    int getHealth() const;
    int getAttack() const;
    bool isAlive() const;

private:
    int team;
    MapTile* tile;
    int x, y;
    int health;
    int damage;
    bool hasMoved;
};

#endif
