#include "game.h"
#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(false), map(nullptr), playerUnit(nullptr), enemyUnit(nullptr), resources(nullptr), L(nullptr) {}

Game::~Game() {
    lua_close(L);
    delete map;
    delete playerUnit;
    delete enemyUnit;
    delete resources;
}

void Game::init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Strategy Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    running = true;

    // Create map, units, and resources
    map = new Map(20, 15);
    playerUnit = new Unit(10, 7, 100, 10);
    enemyUnit = new Unit(5, 5, 50, 5);
    resources = new Resources(20, 15);

    // Initialize Lua
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "lua/ai.lua");
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        running = false;
    }
}

void Game::update() {
    // Get AI's action from Lua script for the enemy
    std::string aiAction = getAIAction(enemyUnit);
    if (aiAction == "collect" && resources->isResourceAt(enemyUnit->getX(), enemyUnit->getY())) {
        resources->collectResource(enemyUnit->getX(), enemyUnit->getY());
    } else {
        enemyUnit->move(aiAction, 20, 15);
    }

    // Simulate combat if units are adjacent
    if (playerUnit->getX() == enemyUnit->getX() && playerUnit->getY() == enemyUnit->getY()) {
        playerUnit->attack(*enemyUnit);
    }
}

std::string Game::getAIAction(Unit* unit) {
    lua_getglobal(L, "decide_unit_action");
    lua_pushlightuserdata(L, unit);
    lua_pushlightuserdata(L, map);
    lua_pushlightuserdata(L, resources);

    if (lua_pcall(L, 3, 1, 0) != 0) {
        std::cerr << "Error in Lua: " << lua_tostring(L, -1) << std::endl;
        return "up";
    }

    std::string action = lua_tostring(L, -1);
    lua_pop(L, 1);
    return action;
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    map->render(renderer);
    playerUnit->render(renderer);
    enemyUnit->render(renderer);
    resources->render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
