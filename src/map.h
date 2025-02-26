#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <vector>
#include "city.h"

class MapTile {
public:
    MapTile(SDL_Color color, SDL_FPoint pos);
    void render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, int depth = 0);
    // Contains all connections
    std::vector<MapTile*> neighbors;
    // Spanning tree children
    std::vector<MapTile*> children;

private:
    SDL_FPoint pos;
    SDL_Color color;
    City* city;
};

class Map {
public:
    Map(int width, int height, float connectedness = 1.0f);
    void render(SDL_Renderer* renderer);

private:
    MapTile* spanningTree;
};

#endif
