#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include "map.h"

MapTile::MapTile(int uuid, SDL_Color color, SDL_FPoint pos) : uuid(uuid), color(color), pos(pos) {
    return;
}

float MapTile::getX(){
    return pos.x;
}
float MapTile::getY(){
    return pos.y;
}

Map::Map(int width, int height, float connectedness) {
    // Generate a grid of MapTiles
    std::vector<std::vector<MapTile*>> grid;
    int uuidCounter = 0;
    for (int x = 0; x < width; x++) {
        std::vector<MapTile*> gridRow;
        for (int y = 0; y < height; y++) {
            // Adjust position for hex grid (offset every other row)
            float offset = (y % 2 == 1) ? 1.5f : 0.0f;
            SDL_FPoint pos = { .x = (float)x * 3.0f + offset, .y = (float)y * 2.598f }; // 2.598 = sqrt(3) * 1.5 for hex spacing
            SDL_Color color = { 0x00, 0x00, 0xff, 0xff };
            MapTile* node = new MapTile(uuidCounter++, color, pos);
            tilePTR.insert({node->uuid, node});
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
// Returns a map tile using its UUID.
MapTile* Map::findNode(int uuid){
    auto search = tilePTR.find(uuid);
    return search->second;
}

int Map::size(){
    return tilePTR.size();
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
    
    std::string text = std::to_string(this->uuid);

    renderText(renderer, font, text, {255,255,255} , vertices[0].position.x, vertices[0].position.y);

    // Recursively render children
    for (MapTile* child : this->children) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, scale * (this->pos.x - pos.x), scale * (this->pos.y - pos.y), scale * (child->pos.x - pos.x), scale * (child->pos.y - pos.y));
        child->render(renderer, pos, scale, depth + 1, font);
    }
}