#include "map.h"
#define _USE_MATH_DEFINES
#include <cmath>

// Map will need to be fully remade to be a graph

MapTile::MapTile(SDL_Color color) : color(color) {
    return;
}

Map::Map(int width, int height) : width(width), height(height) {
    // Create a simple grid with all tiles passable
    grid.resize(height, std::vector<MapTile>(width, MapTile({0, 0, 255, 0})));
}

void Map::render(SDL_Renderer* renderer) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Squash grid vertically, stretch horizontally, and offset every other row by half a column
            SDL_FPoint pos = { .x = 32 * (x * 3.0f + (y % 2) * 1.5f), 
                               .y = 32 * (y * sqrt(0.75)) };
            grid[y][x].render(renderer, pos, 30);
        }
    }
}

bool Map::isValidMove(int x, int y) {
    return true; //(x >= 0 && x < width && y >= 0 && y < height && grid[y][x] == 0);
}

void MapTile::render(SDL_Renderer* renderer, SDL_FPoint pos, float scale) {
    // Create array of hexagonally distributed vertices
    SDL_Vertex vertices[6];
    for (int i = 0; i < 6; i++) {
        vertices[i] = { .position = { .x = pos.x + scale * (cos(i * M_PI / 3)), 
                                      .y = pos.y + scale * (sin(i * M_PI / 3)) },
                        .color = this->color };
    }

    // Create four triangles which cover the surface of the hexagon
    int indices[4 * 3];
    for (int i = 0; i < 4; i++) {
        indices[i * 3 + 0] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }

    // Render hexagon to the screen
    SDL_RenderGeometry(renderer, nullptr, vertices, 6, indices, 12);
}