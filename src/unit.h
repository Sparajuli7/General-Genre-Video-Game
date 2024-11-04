#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>
#include <string>

class Unit {
public:
    Unit(int x, int y, int health, int attack);
    void render(SDL_Renderer* renderer);
    void move(const std::string& direction, int mapWidth, int mapHeight);
    void attack(Unit& target);
    int getX() const;
    int getY() const;
    int getHealth() const;
    bool isAlive() const;

private:
    int x, y;
    int health;
    int attackPower;
};

#endif
