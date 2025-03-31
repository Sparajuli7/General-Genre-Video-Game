

#include "unit.h"
#include <iostream>

// Effective public constructor. Inserts created unit into unit map
Unit* Unit::makeUnit(int health, int attackRatio, MapTile* tile) {
    Unit* unit = new Unit(health, attackRatio, tile);
    units.insert({unit->uuid, unit});
    return unit;
}


// TODO: the units map from game.cpp will be relocated here, this should allow for most code pertaining to manipulating units to reside here.

Unit::Unit(int health, int attackRatio, MapTile* tile) : uuid(Uuid()), health(health), tile(tile) {
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
        SDL_Rect unitRect = { tile->getX() * 32.0, tile->getY() * 32.0, 32, 32 };
        SDL_RenderFillRect(renderer, &unitRect);
    }
}

bool Unit::move(Uuid movingUUID, int targetTileId, Map& map) {
    
    /*
    auto search = units.find(movingUUID);

    if (search == units.end()) {
        std::cout << "Invalid action: Unit " << movingUUID.value << " does not exist." << std::endl;
        return false;
    }
    
    Unit *unit = search->second;

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
        std::cout << "Invalid action: Tile " << targetTileId << " is not adjacent to unit's current tile " << tile->uuid << "." << std::endl;
        return false;
    }

    // Move the unit to the new tile
    tile = targetTile;
    unit->moved = true;
    std::cout << "Unit " << unit->getUUID() << " moved to tile " << targetTileId << "." << std::endl;
    return true;
    */
}

/* HOLY MOLY */
void Unit::attack(Unit& target) {
    target.health -= damage;
}

void Unit::attackUnit(Uuid attackerUUID, Uuid targetUUID)
{
    // Makes sure attackerUUID exists
    if (Unit::getUnits().find(attackerUUID) == Unit::getUnits().end()) {
        std::cout << "Error: Attacker unit UUID " << attackerUUID << " does not exist!" << std::endl;
        return;
    }

    // Making sure the target exists, same as above
    if (Unit::getUnits().find(targetUUID) == Unit::getUnits().end()) {
        std::cout << "Error: Target unit UUID " << targetUUID << " does not exist!" << std::endl;
        return;
    }

    // Gets reference to attacker and target from map
    Unit *attacker = Unit::getUnits().find(attackerUUID)->second;
    Unit *target = Unit::getUnits().find(targetUUID)->second;

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
}

/* HOLY MOLY */




int Unit::getHealth() const {
    return health;
}

int Unit::getAttack() const {
    return damage;
}

bool Unit::isAlive() const {
    return health > 0;
}
