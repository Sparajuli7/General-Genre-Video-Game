#include "player.h"
#include "lua.hpp"
#include "unit.h"  
#include "city.h"   
#include "map.h"
#include "game.h"
#include <iostream>

// Effective public constructor. Inserts created player into player map
Player* Player::makePlayer(bool isBot) {
    Player* player = new Player(isBot);
    players.insert({player->uuid, player});
    return player;
}

Player::Player(bool isBot) : uuid(Uuid()), isBot(isBot) {}


std::deque<GameCommand> Player::takeAction() {
    std::deque<GameCommand> actions;
    if (this->isBot) {
        L = lua::getInitialEnviron("lua/ai.lua");
        lua::runAI(this->L, this->uuid);
    } else {
        Game::requestInput();
    }

    return actions;
}
