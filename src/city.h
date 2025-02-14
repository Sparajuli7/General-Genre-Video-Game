#ifndef CITY_H
#define CITY_H

#include <SDL2/SDL.h>
#include <string>
class MapTile;

class City {
public:
    City(MapTile tile);

private:
    MapTile* tile;
};

#endif
