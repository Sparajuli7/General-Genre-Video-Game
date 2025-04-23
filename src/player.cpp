#include "player.h"
#include "lua.hpp"
#include "unit.h"  
#include "city.h"   
#include "map.h"
#include "game.h"
#include <iostream>

// Effective public constructor. Inserts created player into player map
Player* Player::makePlayer() {
    Player* player = new Player();
    players.insert({player->uuid, player});
    return player;
}

Player::Player() : uuid(Uuid()) {}

std::deque<GameCommand> Player::takeAction() {
    std::deque<GameCommand> actions;
    if (this->isBot) {
        lua::runAI(this->L, this->uuid);
    } else {
        Game::requestInput();
    }

    return actions;
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