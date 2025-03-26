

#include "unit.h"
#include <iostream>

// Effectively the public constructor
Unit& Unit::makeUnit(Uuid uuid, int health, int attackRatio, MapTile* tile) {
    // TODO: This feels dirty. I'm pretty sure there's a better way to do this
    Unit unit = Unit(uuid, health, attackRatio, tile);
    Unit::units.insert({unit.uuid, &unit});
    return *Unit::units.at(unit.uuid);
}


// TODO: the units map from game.cpp will be relocated here, this should allow for most code pertaining to manipulating units to reside here.

Unit::Unit(Uuid uuid, int health, int attackRatio, MapTile* tile) : uuid(Uuid()), health(health), tile(tile) {
    hasMoved = false;
    damage = health * attackRatio;
}

void Unit::renderAll(SDL_Renderer* renderer){
    for (auto itr = units.begin(); itr != Unit::units.end(); ++itr){
        itr->second->render(renderer);
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



// move is not implemented, currently only placeholder code.
bool Unit::move(int targetX, int targetY, Map& map) {

    if (Unit::hasMoved) { 
        std::cout << "Invalid action: This unit has already moved this turn" << std::endl; 
        return -1; 
    }

    //int distance = abs(targetX - x) + abs(targetY - y); 
    if (0) { 
        std::cout << "Invalid action: Can only move up to 5 tiles per turn" << std::endl; 
        return -1; 
    }

    /*
    if (!map.isValidTile(targetX, targetY)) { 
        std::cout << "Invalid action: Selected tile is invalid" << std::endl; 
        return -1; 
    }
    */

    hasMoved = true;

    //std::cout << "Unit moved to (" << x << ", " << y << ")." << std::endl;
    return 0;

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
