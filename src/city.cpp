#include "city.h"

// Constructor: Initializes the city with an ID and assigns its map position
City::City(int id, MapTile* tile) : id(id), tile(tile), unitCreatedThisTurn(false) {}

// Returns the city's ID
int City::getID() const {
    return id;
}

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
        printf("City with ID %d has already created a unit this turn.\n", id);
    }
}

// Resets the turn status to allow unit creation next turn
void City::resetTurn() {
    unitCreatedThisTurn = false;
}

/*this is the test to check if the code works:  Either copy and paste this test ib the file  game.cpp or create another test file: #include "city.h"
#include <iostream>

int main() {
    MapTile* tile = nullptr; // Placeholder for now
    City city1(101, tile); // City belonging to Team 1
    City city2(202, tile); // City belonging to Team 2

    std::cout << "City 1 ID: " << city1.getID() << std::endl;
    std::cout << "City 2 ID: " << city2.getID() << std::endl;

    city1.createUnit(); // Should create a unit
    city1.createUnit(); // Should print that a unit was already created

    city1.resetTurn();  // New turn
    city1.createUnit(); // Should create a unit again

    return 0;
}*/