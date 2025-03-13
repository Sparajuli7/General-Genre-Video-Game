#include "player.h"

Player::Player() {}

// TODO: Actually make use of the Player class.
void Player::render(SDL_Renderer* renderer) {
    return;
}

std::vector<Unit> Player::getUnits() {
    return this->units;
}

std::vector<City> Player::getCities() {
    return this->cities;
}