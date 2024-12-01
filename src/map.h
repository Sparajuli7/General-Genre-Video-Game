#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <vector>

class MapTile {
public:
    MapTile(SDL_Color color);
    void render(SDL_Renderer* renderer, SDL_FPoint pos, float scale);

private:
    SDL_Color color;
};

class Map {
public:
    Map(int width, int height);
    void render(SDL_Renderer* renderer);
    bool isValidMove(int x, int y);
    int dist(int fromX, int fromY, int toX, int toY);
    float weightedDist(int fromX, int fromY, int toX, int toY);

private:
    int width, height;
    std::vector<std::vector<MapTile>> grid;
};

#endif
