

#include "unit.h"
#include "city.h"
#include "player.h"
#include <iostream>

// Effective public constructor. Inserts created unit into unit map
Unit* Unit::makeUnit(int health, int attackRatio, MapTile* tile, Player* owner) {
    // TODO: This feels dirty. I'm pretty sure there's a better way to do this
    Unit *unit = new Unit(health, attackRatio, tile, owner);
    Unit::units.insert({unit->uuid, unit});
    owner->addUnit(unit);
    return Unit::units.at(unit->uuid);
}


// TODO: the units map from game.cpp will be relocated here, this should allow for most code pertaining to manipulating units to reside here.

Unit::Unit(int health, int attackRatio, MapTile* tile, Player* owner) : uuid(Uuid()), health(health), tile(tile), owner(owner) {
    moved = false;
    damage = health * attackRatio;
}

void Unit::renderAll(SDL_Renderer* renderer){
    for (auto const itr : units){
        itr.second->render(renderer);
    }
}

void Unit::render(SDL_Renderer* renderer) {
    if (health > 0) {
        // TODO: Change render logic to render based on location of current tile.
        // Currently crashes on the line that uses tiles, need to see why this is happening.
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect unitRect = { tile->getX() * 30 + 10 + (Unit::owner->getUUID()*21), tile->getY() * 30 + 12, 20, 26 };
        SDL_RenderFillRect(renderer, &unitRect);
    }
}

bool Unit::move(int movingUUID, int targetTileId, Map& map, Player* currentPlayer) {
    
    auto search = units.find(movingUUID);

    if (search == units.end()) {
        std::cout << "Invalid action: Unit " << movingUUID << " does not exist." << std::endl;
        return false;
    }
    
    Unit *unit = search->second;

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
    MapTile* targetTile = map.findNode(targetTileId);
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

    if (!isNeighbor) {
        std::cout << "Invalid action: Tile " << targetTileId << " is not adjacent to unit's current tile " << unit->tile->uuid << "." << std::endl;
        return false;
    }

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


void Unit::attack(Unit& target) {
    target.health -= damage;
}

bool Unit::attackUnit(int attackerUUID, int targetUUID, Player* currentPlayer)
{

    // Makes sure attackerUUID exists
    if (Unit::getUnits().find(attackerUUID) == Unit::getUnits().end()) {
        std::cout << "Error: Attacker unit UUID " << attackerUUID << " does not exist!" << std::endl;
        return false;
    }

    if (Unit::getUnits().find(attackerUUID)->second->getOwner() != currentPlayer){
        std::cout << "Error: Attacker unit UUID " << attackerUUID << " is not your unit!" << std::endl;
        return false;
    }

    

    // Making sure the target exists, same as above
    if (Unit::getUnits().find(targetUUID) == Unit::getUnits().end()) {
        std::cout << "Error: Target unit UUID " << targetUUID << " does not exist! Trying city capture." << std::endl;
        // Check if target is city, perform city actions
        std::map<int, City*> cities = City::getCities();
        if (cities.find(targetUUID) == cities.end()){
            std::cout << "Error: Target city UUID " << targetUUID << " does not exist!" << std::endl;
            return false;
        }
        else {
            Unit *attacker = Unit::getUnits().find(attackerUUID)->second;
            City *target = cities.find(targetUUID)->second;

            if (attacker->getTileUUID() != target->getTileUUID()){
                std::cout << "Error: Target city " << targetUUID << " is not on the same tile as the attacking unit!" << std::endl;
                return false;
            }
            
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

    if (attacker->getTileUUID() != target->getTileUUID()){
        std::cout << "Error: Target unit " << targetUUID << " is not on the same tile as the attacking unit!" << std::endl;
        return false;
    }

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




int Unit::getHealth() const {
    return health;
}

int Unit::getAttack() const {
    return damage;
}

bool Unit::isAlive() const {
    return health > 0;
}
