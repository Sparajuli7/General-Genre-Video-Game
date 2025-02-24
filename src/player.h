#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "unit.h"
#include "city.h"
#include "map.h"

class Player {
public:
    Player();
    void render(SDL_Renderer* renderer);
    void takeAction();

private:
    bool isBot;
    MapTile* cursor;
    std::vector<Unit> units;
    std::vector<City> cities;
};

#endif