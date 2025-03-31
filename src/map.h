#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include "uuid.hpp"

class MapTile {
public:
    MapTile(SDL_Color color, SDL_FPoint pos);
    
    float getX();
    float getY();

    void render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, int depth = 0);
    // Contains all connections
    std::vector<MapTile*> neighbors;
    // Spanning tree children
    std::vector<MapTile*> children;
    const Uuid uuid;

private:
    SDL_FPoint pos;
    SDL_Color color;

    static inline std::map<Uuid, MapTile*> maptiles = std::map<Uuid, MapTile*>();
};

class Map {
public:
    Map(int width, int height, float connectedness = 1.0f);
    //MapTile *findNode(int uuid, MapTile node);
    MapTile *findNode(int uuid);
    int size();
    void render(SDL_Renderer* renderer);
    std::map<Uuid, MapTile*> tilePTR;

private:
    MapTile* spanningTree;
    
};

#endif
