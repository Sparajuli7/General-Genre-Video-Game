#include "resources.h"
#include <cstdlib>

Resources::Resources(int width, int height) : width(width), height(height) {
    // Randomly distribute resources across the map
    resourceGrid.resize(height, std::vector<bool>(width, false));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            resourceGrid[y][x] = (std::rand() % 5 == 0); // 20% chance of a resource
        }
    }
}

void Resources::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for resources
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (resourceGrid[y][x]) {
                SDL_Rect resourceRect = { x * 32, y * 32, 32, 32 };
                SDL_RenderFillRect(renderer, &resourceRect);
            }
        }
    }
}

bool Resources::isResourceAt(int x, int y) {
    return resourceGrid[y][x];
}

void Resources::collectResource(int x, int y) {
    resourceGrid[y][x] = false; // Resource is collected
}
