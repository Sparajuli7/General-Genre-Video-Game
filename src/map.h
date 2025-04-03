#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <vector>
#include <map>
#include <string>

class MapTile {
public:
    MapTile(int uuid, SDL_Color color, SDL_FPoint pos);
    
    // Testing
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y);

    float getX();
    float getY();

    void render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, int depth = 0, TTF_Font* font = NULL);
    // Contains all connections
    std::vector<MapTile*> neighbors;
    // Spanning tree children
    std::vector<MapTile*> children;
    int uuid;

    SDL_Texture *texture, *text;
    TTF_Font* font;


private:
    SDL_FPoint pos;
    SDL_Color color;
};

class Map {
public:
    Map(int width, int height, float connectedness = 1.0f);
    //MapTile *findNode(int uuid, MapTile node);
    MapTile *findNode(int uuid);
    int size();
    void render(SDL_Renderer* renderer, TTF_Font* font);
    std::map<int, MapTile*> tilePTR;

private:
    MapTile* spanningTree;
    
};

#endif
