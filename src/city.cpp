#include "city.h"

// Effective public constructor. Inserts created city into city map
City* City::makeCity(MapTile* tile) {
    City* city = new City(tile);
    cities.insert({city->uuid, city});
    return city;
}

// Constructor: Initializes the city with an ID and assigns its map position
City::City(MapTile* tile) : uuid(Uuid()), tile(tile), unitCreatedThisTurn(false) {}

City::~City() {
    cities.erase(this->uuid);
}

// Returns the city's ID
int City::getID() const {
    return uuid;
}

// Returns the city's location on the map.
int City::getTileID() const{
    return tile->uuid;
}

// Checks if the city is allowed to create a unit this turn
bool City::canCreateUnit() const {
    return !unitCreatedThisTurn;
}

// Creates a unit if allowed
Unit *City::createUnit(int health, int attack) {
    if (!unitCreatedThisTurn) {
        unitCreatedThisTurn = true;

        auto units = Unit::getUnits();
        
        // Should work with multiple cities but isn't pretty.
        for (int i = uuid+1; i < uuid+97; i++){
            auto search = units.find(i);
            if (search != units.end()){
                continue;
            }
            else{
                printf("Unit created by city with ID %d\n", i);
                return Unit::makeUnit(health, attack, tile);
            }
        }
        // TODO: Need way to check if ID already exists.
        
        // return Unit();
        
    } else {
        printf("City with ID %d has already created a unit this turn.\n", uuid);
        return NULL;
    }
    return NULL;
}

// Resets the turn status to allow unit creation next turn
void City::resetTurn() {
    unitCreatedThisTurn = false;
}
