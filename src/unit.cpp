#include "unit.h"
#include "city.h"
#include "player.h"
#include <iostream>

// Effective public constructor. Inserts created unit into unit map
Unit* Unit::makeUnit(int health, int attackRatio, MapTile* tile, Player* owner) {
    Unit *unit = new Unit(health, attackRatio, tile, owner);
    Unit::units.insert({unit->uuid, unit});
    owner->addUnit(unit);
    return Unit::units.at(unit->uuid);
}

Unit::Unit(int health, int attackRatio, MapTile* tile, Player* owner) : uuid(Uuid()), health(health), tile(tile), owner(owner) {
    moved = false;
    damage = health * attackRatio;
}

// Renders every unit in the units map
void Unit::renderAll(SDL_Renderer* renderer){
    for (auto const itr : units){
        itr.second->render(renderer);
    }
}

void Unit::render(SDL_Renderer* renderer) {
    if (health > 0) {
        // Having various values here be a modifiable variable would be needed to make the window size changable
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect unitRect = { (int) (tile->getX() * 30) + 10 + (Unit::owner->getUUID()*21), (int) (tile->getY() * 30) + 12, 20, 26 };
        SDL_RenderFillRect(renderer, &unitRect);
    }
}

bool Unit::move(int movingUUID, int targetTileId, Map& map, Player* currentPlayer) {
    
    auto search = units.find(movingUUID);

    // Check if unit with UUID exists
    if (search == units.end()) {
        std::cout << "Invalid action: Unit " << movingUUID << " does not exist." << std::endl;
        return false;
    }
    
    Unit *unit = search->second;

    // Check if unit is owned by current player
    if (unit->getOwner() != currentPlayer){
        std::cout << "Error: Unit " << movingUUID << " is not your unit!" << std::endl;
        return false;
    }

    // Check if the unit has already moved this turn
    if (unit->hasMoved()) {
        std::cout << "Invalid action: Unit " << unit->getUUID() << " has already moved this turn." << std::endl;
        return false;
    }

    // Check if the unit is trying to move to its current tile
    if (unit->tile->uuid == targetTileId) {
        std::cout << "Unit " << unit->getUUID() << " is already on tile " << targetTileId << "." << std::endl;
        return false;
    }

    // Find the destination tile using the map
    MapTile* targetTile = MapTile::uuidToMaptile(targetTileId);
    if (!targetTile) {
        std::cout << "Invalid action: Tile with ID " << targetTileId << " does not exist." << std::endl;
        return false;
    }

    // Check if the target tile is a neighbor
    bool isNeighbor = false;
    for (MapTile* neighbor : unit->tile->neighbors) {
        if (neighbor && neighbor->uuid == targetTileId) {
            isNeighbor = true;
            break;
        }
    }

    // Check if the unit is adjecent to the target tile
    if (!isNeighbor) {
        std::cout << "Invalid action: Tile " << targetTileId << " is not adjacent to unit's current tile " << unit->tile->uuid << "." << std::endl;
        return false;
    }

    // Check if there the player already has a unit on the target tile
    for (auto itr : unit->owner->getUnits()){
        if (itr->getTileUUID() == targetTileId){
            std::cout << "Invalid action: Tile " << targetTileId << " already has your unit " << itr->getUUID() << "on it." << std::endl;
            return false;
        }
    }

    // Move the unit to the new tile
    unit->tile = targetTile;
    unit->moved = true;
    std::cout << "Unit " << unit->getUUID() << " moved to tile " << targetTileId << "." << std::endl;
    return true;
}

bool Unit::attackUnit(int attackerUUID, int targetUUID, Player* currentPlayer)
{

    // Check if attacker unit exists
    if (Unit::getUnits().find(attackerUUID) == Unit::getUnits().end()) {
        std::cout << "Error: Attacker unit UUID " << attackerUUID << " does not exist!" << std::endl;
        return false;
    }

    // Check if attacker unit is owned by the player
    if (Unit::getUnits().find(attackerUUID)->second->getOwner() != currentPlayer){
        std::cout << "Error: Attacker unit UUID " << attackerUUID << " is not your unit!" << std::endl;
        return false;
    }

    

    // Making sure the target exists, same as above
    if (Unit::getUnits().find(targetUUID) == Unit::getUnits().end()) {
        std::cout << "Error: Target unit UUID " << targetUUID << " does not exist! Trying city capture." << std::endl;
        
        // Check if target is city, perform city actions instead
        std::map<int, City*> cities = City::getCities();
        if (cities.find(targetUUID) == cities.end()){
            std::cout << "Error: Target city UUID " << targetUUID << " does not exist!" << std::endl;
            return false;
        }
        else {
            Unit *attacker = Unit::getUnits().find(attackerUUID)->second;
            City *target = cities.find(targetUUID)->second;

            // Check if city is on the same tile
            if (attacker->getTileUUID() != target->getTileUUID()){
                std::cout << "Error: Target city " << targetUUID << " is not on the same tile as the attacking unit!" << std::endl;
                return false;
            }
            
            // Check if city is owned by the same player
            if(target->getOwner() == attacker->getOwner()){
                std::cout << "Error: Cannot capture city on the same team!" << std::endl;
                return false;
            } else{
                Player* oldOwner = target->getOwner();
                oldOwner->removeCity(target);
                target->setOwner(attacker->getOwner());
                attacker->getOwner()->addCity(target);
                std::cout << "City " << target->getUUID() << " captured!\n";
                return true;
            }
        }
        
    }

    // Gets reference to attacker and target from map
    Unit *attacker = Unit::getUnits().find(attackerUUID)->second;
    Unit *target = Unit::getUnits().find(targetUUID)->second;

    // Check if target unit is on the same tile as the attacker unit
    if (attacker->getTileUUID() != target->getTileUUID()){
        std::cout << "Error: Target unit " << targetUUID << " is not on the same tile as the attacking unit!" << std::endl;
        return false;
    }

    // Check if target is also owned the player
    if (attacker->getOwner() == target->getOwner()){
        std::cout << "Error: Cannot attack units on the same team!" << std::endl;
        return false;
    }

    // Calls the attack
    attacker->attack(*target);

    // Show remaining HP
    std::cout << "Unit " << attackerUUID << " attacked unit " << targetUUID 
            << "! Remaining HP: " << target->getHealth() << std::endl;

    // See if unit is killed
    if (!target->isAlive()) {
        //Remove dead player from map
        std::cout << "Unit " << targetUUID << " has been defeated and will be removed from the game." << std::endl;
        Unit::getUnits().erase(int(targetUUID));
    }

    return true;
}

