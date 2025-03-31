#include "player.h"

// Effective public constructor. Inserts created player into player map
Player* Player::makePlayer() {
    Player* player = new Player();
    players.insert({player->uuid, player});
    return player;
}

Player::Player() : uuid(Uuid()) {}

Player::~Player() {
    players.erase(this->uuid);
}

// TODO: Actually make use of the Player class.
void Player::render(SDL_Renderer* renderer) {
    return;
}

std::vector<Unit*> Player::getUnits() {
    return this->units;
}

std::vector<City*> Player::getCities() {
    return this->cities;
}