// Units have been removed from game.cpp until graph map is implemented

#include "unit.h"

Unit::Unit(int id, int health, int attackRatio, MapTile* tile) : id(id), health(health), tile(tile) {
    hasMoved = false;
    damage = health * attackRatio;
}

void Unit::render(SDL_Renderer* renderer) {
    if (health > 0) {
        // TODO: Change render logic to render based on location of current tile.
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_Rect unitRect = { x * 32, y * 32, 32, 32 };
        //SDL_RenderFillRect(renderer, &unitRect);
    }
}

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
