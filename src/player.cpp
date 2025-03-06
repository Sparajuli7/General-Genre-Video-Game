#include "player.h"

Player::Player() {}

void Player::render(SDL_Renderer* renderer) {
    return;
}

std::vector<Unit> Player::getUnits() {
    return this->units;
}

std::vector<City> Player::getCities() {
    return this->cities;
}