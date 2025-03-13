#include "player.h"

// Effective public constructor
Player& Player::makePlayer() {
    // TODO: This feels dirty. I'm pretty sure there's a better way to do this
    Player player = Player();
    players.insert({player.uuid, player});
    return players.at(player.uuid);
}

Player::Player() : uuid(Uuid()) {}

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