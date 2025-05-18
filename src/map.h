#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <vector>
#include <map>
#include <string>
#include "uuid.hpp"

class MapTile {
public:
    ~MapTile();

    static MapTile* makeMapTile(SDL_Color color, SDL_FPoint pos);
    static std::pair<int, std::vector<MapTile*>> path(MapTile *a, MapTile *b);
    static MapTile* uuidToMaptile(Uuid uuid) { return mapTiles.at(uuid); };
    static const std::map<Uuid, MapTile*>& getMapTiles() { return mapTiles; };
    
    // Testing
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y);

    float getX() { return this->pos.x; };
    float getY() { return this->pos.y; };
    const std::vector<MapTile*> getNeighbors() { return this->neighbors; };

    void render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, int depth = 0, TTF_Font* font = NULL);

    // Contains all connections
    std::vector<MapTile*> neighbors;
    // Spanning tree children
    std::vector<MapTile*> children;
    const Uuid uuid;

    SDL_Texture *texture, *text;
    TTF_Font* font;

private:
    MapTile(SDL_Color color, SDL_FPoint pos);
    static inline std::map<Uuid, MapTile*> mapTiles = std::map<Uuid, MapTile*>();

    SDL_FPoint pos;
    SDL_Color color;
};

class Map {
public:
    Map(int width, int height, float connectedness = 1.0f);
    int size() { return MapTile::getMapTiles().size(); };
    void render(SDL_Renderer* renderer, TTF_Font* font);

private:
    MapTile* spanningTree;
};

#endif
