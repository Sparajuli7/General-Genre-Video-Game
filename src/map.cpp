#include "map.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include <iostream>

MapTile::MapTile(SDL_Color color, SDL_FPoint pos) : color(color), pos(pos) {
    return;
}

Map::Map(int width, int height, float connectedness) {
    // Generate a grid of MapTiles
    std::vector<std::vector<MapTile*>> grid;
    for (int x = 0; x < width; x++) {
        std::vector<MapTile*> gridRow;
        for (int y = 0; y < width; y++) {
            SDL_FPoint pos = { .x = (float) x * 3,
                               .y = (float) y * 3 };
            SDL_Color color = { 0x00, 0x00, 0xff, 0x00 };
            MapTile* node = new MapTile(color, pos);
            gridRow.push_back(node);
        }
        grid.push_back(gridRow);
    }

    // Zig zag through grid to create spanning tree
    // TODO: Implement breadth first search to do this instead
    // TODO: Probably better to have an intermediate representation where we directly work with edges
    this->spanningTree = grid[0][0];
    MapTile* node = grid[0][0];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            MapTile* nextNode;
            // Skip first node
            if (x == 0 && y == 0)
                continue;
            // Alternate direction every other column
            if (x % 2 == 0) {
                nextNode = grid[x][y];
            } else {
                nextNode = grid[x][height - (y + 1)];
            }
            node->children.push_back(nextNode);
            node = nextNode;
        }
    }
}

void Map::render(SDL_Renderer* renderer) {
    this->spanningTree->render(renderer, {-1.0f, -1.0f}, 20.0f);
}

//
void MapTile::render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, int depth) {
    // TODO: Creating the hexagon each iteration shouldn't be necessary
    // Create array of hexagonally distributed vertices
    const int numVertices = 6;
    SDL_Vertex vertices[numVertices];
    for (int i = 0; i < numVertices; i++) {
        vertices[i] = { .position = { .x = scale * (this->pos.x - pos.x + cos(i * M_PI / 3)), 
                                      .y = scale * (this->pos.y - pos.y + sin(i * M_PI / 3)) },
                        .color = this->color };
    }

    // Create four triangles which cover the surface of the hexagon
    const int numTriangles = 4;
    const int numIndices = numTriangles * 3;
    int indices[numIndices];
    for (int i = 0; i < numTriangles; i++) {
        indices[i * 3 + 0] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }

    // Render hexagon to the screen
    SDL_RenderGeometry(renderer, nullptr, vertices, numVertices, indices, numIndices);

    // Recursively render children
    for (MapTile* child : this->children) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, scale * (this->pos.x - pos.x), scale * (this->pos.y - pos.y), scale * (child->pos.x - pos.x), scale * (child->pos.y - pos.y));
        child->render(renderer, pos, scale, depth + 1);
    }
}