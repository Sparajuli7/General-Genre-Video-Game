#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include "map.h"

// Effective public constructor
MapTile* MapTile::makeMapTile(SDL_Color color, SDL_FPoint pos) {
    MapTile* mapTile = new MapTile(color, pos);
    mapTiles.insert({mapTile->uuid, mapTile});
    return mapTile;
}

MapTile::MapTile(SDL_Color color, SDL_FPoint pos) : uuid(Uuid()), color(color), pos(pos) { }

Map::Map(int width, int height, float connectedness) {
    // Generate a grid of MapTiles
    std::vector<std::vector<MapTile*>> grid;
    for (int x = 0; x < width; x++) {
        std::vector<MapTile*> gridRow;
        for (int y = 0; y < height; y++) {
            // Adjust position for hex grid (offset every other row)
            float offset = (y % 2 == 1) ? 1.5f : 0.0f;
            SDL_FPoint pos = { .x = (float)x * 3.0f + offset, .y = (float)y * 2.598f }; // 2.598 = sqrt(3) * 1.5 for hex spacing
            SDL_Color color = { 0x00, 0x00, 0xff, 0xff };
            MapTile* node = MapTile::makeMapTile(color, pos);
            gridRow.push_back(node);
        }
        grid.push_back(gridRow);
    }

    // Populate neighbors for each tile in a hex grid
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            MapTile* tile = grid[x][y];
            tile->neighbors.resize(6, nullptr); // 6 possible neighbors in a hex grid

            // Neighbor indices: 0 = NE, 1 = E, 2 = SE, 3 = SW, 4 = W, 5 = NW
            bool isOddRow = (y % 2 == 1);
            if (y % 2 == 0) { // Even rows
                if (y > 0) {
                    if (x < width - 1) tile->neighbors[0] = grid[x + 1][y - 1]; // NE
                    tile->neighbors[5] = grid[x][y - 1]; // NW
                }
                if (x < width - 1) tile->neighbors[1] = grid[x + 1][y]; // E
                if (y < height - 1) {
                    if (x < width - 1) tile->neighbors[2] = grid[x + 1][y + 1]; // SE
                    tile->neighbors[3] = grid[x][y + 1]; // SW
                }
                if (x > 0) tile->neighbors[4] = grid[x - 1][y]; // W
            } else { // Odd rows
                if (y > 0) {
                    tile->neighbors[0] = grid[x][y - 1]; // NE
                    if (x > 0) tile->neighbors[5] = grid[x - 1][y - 1]; // NW
                }
                if (x < width - 1) tile->neighbors[1] = grid[x + 1][y]; // E
                if (y < height - 1) {
                    tile->neighbors[2] = grid[x][y + 1]; // SE
                    if (x > 0) tile->neighbors[3] = grid[x - 1][y + 1]; // SW
                }
                if (x > 0) tile->neighbors[4] = grid[x - 1][y]; // W
            }
        }
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

// Compute path and distance between two map tiles
// Uses A* algorithm with zero heuristic
std::pair<int, std::vector<MapTile*>> MapTile::path(MapTile *a, MapTile *b) {
    // Map of nodes to check, keyed by their current best path length
    std::multimap<int, MapTile*> openSet = {{0, a}};
    // Map of best path lengths, keyed by node
    std::map<MapTile*, int> scores;
    // Map of previous node in best path, keyed by node
    std::map<MapTile*, MapTile*> prev;

    // Return early if start and end are identical
    if (a == b) {
        return std::make_pair(0, std::vector<MapTile*>());
    }

    while (!openSet.empty()) {
        // Find current best candidate
        auto current = openSet.begin();

        // If candidate matches target, we're done
        if (current->second == b) {
            int length = current->first;

            std::vector<MapTile*> path(length);
            MapTile* node = current->second;
            for (int i = length - 1; i >= 0; i--) {
                path.at(i) = node;
                node = prev.at(node);
            }

            return std::make_pair(length, path);
        }

        // Otherwise, search neighbors
        for (const auto &neighbor : current->second->neighbors) {
            // Map generation is busted, some neighbor values are null
            if (neighbor == nullptr) {
                continue;
            }

            // Each vertex has a weight of one
            int tentativeScore = current->first + 1;

            // If new path is shorter, update scores and prev
            // Also, short circuit evaluation is important here
            if (scores.find(neighbor) == scores.end() || tentativeScore < scores.at(neighbor)) {
                scores.insert_or_assign(neighbor, tentativeScore);
                prev.insert_or_assign(neighbor, current->second);

                // Remove node from open set if it exists
                auto it = openSet.begin();
                for (auto it = openSet.begin(); it != openSet.end(); it++) {
                    if (it->second == neighbor) {
                        openSet.erase(it);
                        break;
                    }
                }

                // Insert node into open set with potentially updated score
                openSet.insert({tentativeScore, neighbor});
            }
        }

        // Finally, remove candidate from queue
        openSet.erase(current);
    }

    // No path found
    return std::make_pair(0, std::vector<MapTile*>());
}

void MapTile::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y) {
    // Create surface from text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        // Handle error
        return;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        // Handle error
        SDL_FreeSurface(textSurface);
        return;
    }

    // Set up the destination rectangle
    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};

    // Render the texture
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void Map::render(SDL_Renderer* renderer, TTF_Font* font) {
    this->spanningTree->render(renderer, {-1.0f, -1.0f}, 30.0f, 0, font);
}

//
void MapTile::render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, int depth, TTF_Font* font) {
    // TODO: Creating the hexagon each iteration shouldn't be necessary
    // Create array of hexagonally distributed vertices
    const int numVertices = 6;
    SDL_Vertex vertices[numVertices];
    for (int i = 0; i < numVertices; i++) {
        vertices[i] = { .position = { .x = scale * (this->pos.x - pos.x + (float) cos(i * M_PI / 3)), 
                                      .y = scale * (this->pos.y - pos.y + (float) sin(i * M_PI / 3)) },
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
    
    
    std::string text = std::to_string(this->uuid);

    renderText(renderer, font, text, {255,255,255} , vertices[0].position.x-2, vertices[0].position.y+10);

    // Recursively render children
    for (MapTile* child : this->children) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, scale * (this->pos.x - pos.x), scale * (this->pos.y - pos.y), scale * (child->pos.x - pos.x), scale * (child->pos.y - pos.y));
        
        child->render(renderer, pos, scale, depth + 1, font);
    }
    SDL_RenderGeometry(renderer, nullptr, vertices, numVertices, indices, numIndices);
}