#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include <vector>
#include <map>
#include <string>
#include <unordered_set>
#include <random>

class MapTile {
public:
    MapTile(int uuid, SDL_Color color, SDL_FPoint pos);
    
    const int getUUID() { return uuid; } ;
    const float getX() { return pos.x; } ;
    const float getY() { return pos.y; } ;

    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y);
    void render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, TTF_Font* font, int mode);

    // Contains all connections.
    std::vector<MapTile*> neighbors;
    
private:
    int uuid;
    SDL_FPoint pos;
    SDL_Color color;   
};

class Map {
public:
    Map(int width, int height, float connectedness = 1.0f);

    const int size() { return tilePTR.size(); };

    MapTile *findNode(int uuid);
    
    void render(SDL_Renderer* renderer, TTF_Font* font);
    
    // Contains all MapTiles and their UUID.
    std::map<int, MapTile*> tilePTR;

private:
    bool randomCheck(int chance = 25);
};

#endif
