#include "unit.h"

Unit::Unit(int x, int y, int health, int attack) : x(x), y(y), health(health), attackPower(attack) {}

void Unit::render(SDL_Renderer* renderer) {
    if (health > 0) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect unitRect = { x * 32, y * 32, 32, 32 };
        SDL_RenderFillRect(renderer, &unitRect);
    }
}

void Unit::move(const std::string& direction, int mapWidth, int mapHeight) {
    if (direction == "up" && y > 0) y--;
    if (direction == "down" && y < mapHeight - 1) y++;
    if (direction == "left" && x > 0) x--;
    if (direction == "right" && x < mapWidth - 1) x++;
}

void Unit::attack(Unit& target) {
    target.health -= attackPower;
}

int Unit::getX() const {
    return x;
}

int Unit::getY() const {
    return y;
}

int Unit::getHealth() const {
    return health;
}

bool Unit::isAlive() const {
    return health > 0;
}
