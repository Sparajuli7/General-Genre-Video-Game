#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL2/SDL.h>
#include <vector>

class Resources {
public:
    Resources(int width, int height);
    void render(SDL_Renderer* renderer);
    bool isResourceAt(int x, int y);
    void collectResource(int x, int y);

private:
    std::vector<std::vector<bool>> resourceGrid;
    int width, height;
};

#endif
