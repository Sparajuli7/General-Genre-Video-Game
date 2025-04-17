#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include "map.h"

/* ~~~ MapTile ~~~ */
// MapTiles contain information about a tile in the map graph.

// MapTile constructor
MapTile::MapTile(int uuid, SDL_Color color, SDL_FPoint pos) : uuid(uuid), color(color), pos(pos) {}

// Function that makes rendering text easier, only used here at the moment, but should be moved elsewhere if used in other classes.
void MapTile::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y) {
    // Create surface from text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cout << "Something went wrong while trying to create text surface!" << std::endl;
        return;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cout << "Something went wrong while trying to create text texture!" << std::endl;
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

// Renders either an individual tile or lines to its neighbors, this can be set with the mode variable.
// A tile is represented as a hexagon.
// Lines = 1
// Tiles = 0
void MapTile::render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, TTF_Font* font, int mode) {
    // Line rendering
    if (mode == 1){
        for (MapTile* neighbor : this->neighbors) {
            if (neighbor == nullptr) continue;
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE); // White line
            // (tile->pos.x/y - pos.x/y) makes the line be in the center of the hexagon for some reason.
            SDL_RenderDrawLine(renderer, scale * (this->pos.x - pos.x), scale * (this->pos.y - pos.y), scale * (neighbor->pos.x - pos.x), scale * (neighbor->pos.y - pos.y));
        }
        return;
    }
    // Hexagon (tile) rendering
    else{
        // Magic math that makes a hexagon
        const int numVertices = 6;
        SDL_Vertex vertices[numVertices];
        for (int i = 0; i < numVertices; i++) {
            vertices[i] = { .position = { .x = scale * (this->pos.x - pos.x + float(cos(i * M_PI / 3))), 
                            .y = scale * (this->pos.y - pos.y + float(sin(i * M_PI / 3))) },
                            .color = this->color };
        }
        // More magic math that makes triangles into a hexagon. 
        const int numTriangles = 4;
        const int numIndices = numTriangles * 3;            
        int indices[12] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5
        };

        // Create text that provides the tile's UUID next to the hexagon.
        std::string text = std::to_string(this->uuid);
        renderText(renderer, font, text, {255,64,255} , vertices[0].position.x-2, vertices[0].position.y+10);
        // Render hexagon to the screen.
        SDL_RenderGeometry(renderer, nullptr, vertices, numVertices, indices, numIndices);
        return;
    }
    return;
}

/* ~~~ MapTile END ~~~ */

/* ~~~ Map ~~~ */
// The map contains a graph of all tiles and their neighbors

// Constructor to create a graph of tiles. TODO: refractor and clean up and add comments
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
            tilePTR.insert({node->getUUID(), node});
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
                    if ((x < width - 1)) tile->neighbors[0] = grid[x + 1][y - 1]; // NE
                    tile->neighbors[5] = grid[x][y - 1]; // NW
                }
                if ((x < width - 1)) tile->neighbors[1] = grid[x + 1][y]; // E
                if ((y < height - 1)) {
                    if ((x < width - 1)) tile->neighbors[2] = grid[x + 1][y + 1]; // SE
                    tile->neighbors[3] = grid[x][y + 1]; // SW
                }
                if ((x > 0)) tile->neighbors[4] = grid[x - 1][y]; // W
            } else { // Odd rows
                if ((y > 0)) {
                    tile->neighbors[0] = grid[x][y - 1]; // NE
                    if ((x > 0)) tile->neighbors[5] = grid[x - 1][y - 1]; // NW
                }
                if ((x < width - 1)) tile->neighbors[1] = grid[x + 1][y]; // E
                if ((y < height - 1)) {
                    tile->neighbors[2] = grid[x][y + 1]; // SE
                    if ((x > 0)) tile->neighbors[3] = grid[x - 1][y + 1]; // SW
                }
                if ((x > 0)) tile->neighbors[4] = grid[x - 1][y]; // W
            }
        }
    }
}

// Returns a map tile using its UUID.
MapTile* Map::findNode(int uuid){
    auto search = tilePTR.find(uuid);
    return search->second;
}

// Random chance out of 100 
bool Map::randomCheck(int chance){
    int pickedChoice = (int)(rand() * 100);
    if(pickedChoice <= chance){
        return true;
    } else return false;

}

// Starts by rendering all tile connections, then the tiles themselves.
void Map::render(SDL_Renderer* renderer, TTF_Font* font) {
    for(auto itr : tilePTR){
        itr.second->render(renderer, {-1.0f, -1.0f}, 30.0f, font, 1);
    }
    for(auto itr : tilePTR){
        itr.second->render(renderer, {-1.0f, -1.0f}, 30.0f, font, 0);
    }
}

/* ~~~ Map END ~~~ */

// Old Recursive Rendering method, keeping for now.
/*
void MapTile::render(SDL_Renderer* renderer, SDL_FPoint pos, float scale, int depth, TTF_Font* font, std::unordered_set<int>* visited, int mode) {
    // TODO: Creating the hexagon each iteration shouldn't be necessary
    // Create array of hexagonally distributed vertices

    if (visited->find(this->uuid) != visited->end()) {
        return;
    }
    
    // Mark this tile as visited
    visited->insert(this->uuid);

    if (mode == 1){
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
        /*int indices[numIndices];
        for (int i = 0; i < numTriangles; i++) {
            indices[i * 3 + 0] = 0;
            indices[i * 3 + 1] = i + 1;
            indices[i * 3 + 2] = i + 2;
        }

        
        int indices[12] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5
        };
        // Render hexagon to the screen
        
        
        std::string text = std::to_string(this->uuid);

        renderText(renderer, font, text, {255,64,255} , vertices[0].position.x-2, vertices[0].position.y+10);

        SDL_RenderGeometry(renderer, nullptr, vertices, numVertices, indices, numIndices);
    }

    // Recursively render children
    for (MapTile* child : this->children) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, scale * (this->pos.x - pos.x), scale * (this->pos.y - pos.y), scale * (child->pos.x - pos.x), scale * (child->pos.y - pos.y));
        
        child->render(renderer, pos, scale, depth + 1, font);
    }

    for (MapTile* neighbor : this->neighbors) {
        if (neighbor == nullptr) continue;;
        if (mode == 0){
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, scale * (this->pos.x - pos.x), scale * (this->pos.y - pos.y), scale * (neighbor->pos.x - pos.x), scale * (neighbor->pos.y - pos.y));
        }
        //std::cout << neighbor->uuid << std::endl;
        neighbor->render(renderer, pos, scale, depth + 1, font, visited, mode);
    }
}
*/
