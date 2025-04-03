#include "city.h"


// Effective public constructor. Inserts created city into city map
City* City::makeCity(MapTile* tile) {
    City* city = new City(tile);
    cities.insert({city->uuid, city});
    return city;
}

// Constructor: Initializes the city with an ID and assigns its map position
City::City(MapTile* tile) : uuid(Uuid()), tile(tile), unitCreatedThisTurn(false) {}

// Checks if the city is allowed to create a unit this turn
bool City::canCreateUnit() const {
    return !unitCreatedThisTurn;
}

// Creates a unit if allowed
bool City::createUnit(int cityUUID, int health, int attack) {
    
    auto search = cities.find(cityUUID);

    if (search == cities.end()) {
        std::cout << "Invalid action: Unit " << cityUUID << " does not exist." << std::endl;
        return false;
    }
    
    City *city = search->second;
    
    if (!city->unitCreatedThisTurn) {
        city->unitCreatedThisTurn = true;
        
        auto unit = Unit::makeUnit(health, attack, city->tile);

        printf("Unit created by city with ID %d\n", unit->getUUID());
        return true;

        // TODO: Need way to check if ID already exists.
        
        // return Unit();
        
    } else {
        printf("City with ID %d has already created a unit this turn.\n", city->uuid);
        return false;
    }
    return false;
}

// Resets the turn status to allow unit creation next turn
void City::resetTurn() {
    unitCreatedThisTurn = false;
}

void City::renderAll(SDL_Renderer* renderer){
    for (auto const itr : cities){
        itr.second->render(renderer);
    }
}

void City::render(SDL_Renderer* renderer) {
    // TODO: Change render logic to render based on location of current tile.
    // Currently crashes on the line that uses tiles, need to see why this is happening.
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect cityRect = { tile->getX() * 30 + 15, tile->getY() * 30 + 45, 28, 10 };
    SDL_RenderFillRect(renderer, &cityRect);
    
}
