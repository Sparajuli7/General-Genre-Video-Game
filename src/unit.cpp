

#include "unit.h"
#include <iostream>

// Effectively the public constructor
Unit& Unit::makeUnit() {
    // TODO: This feels dirty. I'm pretty sure there's a better way to do this
    Unit unit = Unit();
    Unit::units.insert({unit.uuid, unit});
    return Unit::units.at(unit.uuid);
}

Unit::Unit() : uuid(Uuid()) {}

// TODO: the units map from game.cpp will be relocated here, this should allow for most code pertaining to manipulating units to reside here.

Unit::Unit(int id, int health, int attackRatio, MapTile* tile) : id(id), health(health), tile(tile) {
    hasMoved = false;
    damage = health * attackRatio;
}

void Unit::render(SDL_Renderer* renderer) {
    if (health > 0) {
        // TODO: Change render logic to render based on location of current tile.
        // Currently doesn't render unit location correctly.
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect unitRect = { tile->getX() * 32, tile->getY() * 32, 32, 32 };
        SDL_RenderFillRect(renderer, &unitRect);
    }
}

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

// To be expanded apon
void Unit::attack(Unit& target) {
    target.health -= damage;
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
