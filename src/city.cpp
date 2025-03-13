#include "city.h"

// Effective public constructor
City& City::makeCity() {
    // TODO: This feels dirty. I'm pretty sure there's a better way to do this
    City city = City();
    cities.insert({city.uuid, city});
    return cities.at(city.uuid);
}

City::City() : uuid(Uuid()) {}

// Constructor: Initializes the city with an ID and assigns its map position
City::City(Uuid uuid, MapTile* tile) : uuid(uuid), tile(tile), unitCreatedThisTurn(false) {}

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
Unit *City::createUnit(std::map<int, Unit*> units, int health, int attack) {
    if (!unitCreatedThisTurn) {
        unitCreatedThisTurn = true;
        
        // Should work with multiple cities but isn't pretty.
        for (int i = id+1; i < id+97; i++){
            auto search = units.find(i);
            if (search != units.end()){
                continue;
            }
            else{
                return new Unit(i, health, attack, tile);
            }
        }
        // TODO: Need way to check if ID already exists.
        
        // return Unit();
        printf("Unit created by city with ID %d\n", id);
    } else {
        printf("City with ID %d has already created a unit this turn.\n", uuid);
    }
}

// Resets the turn status to allow unit creation next turn
void City::resetTurn() {
    unitCreatedThisTurn = false;
}
