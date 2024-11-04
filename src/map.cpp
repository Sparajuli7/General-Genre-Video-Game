#include "map.h"

// Map will need to be fully remade to be a graph

Map::Map(int width, int height) : width(width), height(height) {
    // Create a simple grid with all tiles passable
    grid.resize(height, std::vector<int>(width, 0));
}

void Map::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_Rect tile = { x * 32, y * 32, 32, 32 };
            SDL_RenderDrawRect(renderer, &tile);
        }
    }
}

bool Map::isValidMove(int x, int y) {
    return (x >= 0 && x < width && y >= 0 && y < height && grid[y][x] == 0);
}
