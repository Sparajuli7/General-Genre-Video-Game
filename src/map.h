#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <vector>

class Map {
public:
    Map(int width, int height);
    void render(SDL_Renderer* renderer);
    bool isValidMove(int x, int y);
    
private:
    int width, height;
    std::vector<std::vector<int>> grid;
};

#endif
