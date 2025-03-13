#include "game.h"
#include "lua.h"

// unit ids (1xx for team 1, 2xx for team 2)
// figure out how data is stored and shared between lua and C
// Shared pointers

Game::Game() : window(nullptr), renderer(nullptr), running(false), map(nullptr), L(nullptr) {}

Game::~Game() {

    delete map;

}

void Game::init() {
    // Create SDL window and renderer
    //SDL_Init(SDL_INIT_VIDEO);
    //window = SDL_CreateWindow("Strategy Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    //renderer = SDL_CreateRenderer(window, -1, 0);
    running = true;

    Game::round = 0;
    

    // Create map

    map = new Map(10, 10);

    Unit::makeUnit();
    Unit::makeUnit();
    Unit::makeUnit();
    Unit::makeUnit();
    lua_State* L = lua::getInitialEnviron("lua/ai.lua");
    lua::runAI(L, 0);

}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        running = false;
    }
}

void Game::update() {

    Game::round++;
    std::string command = "";
    bool move_status = false;

    printf("Round %d begins...\n", Game::round);

    while(command != "done"){
        printf("Enter command: ");
        getline(std::cin, command);
        // validate move does nothing atm.

        switch (convertToImmediate(command)) {
            case ImmediateCommands::done:
                break;
            case ImmediateCommands::undo:
                std::cout << "Reversing command: " << Game::actions.front() << std::endl;
                Game::actions.pop_front();
                break;
            case ImmediateCommands::listmap: // Returns the map and provides info on what units and cities are in each tile
                break;
            case ImmediateCommands::listunit: // Returns the specified player's units
                break;
            case ImmediateCommands::listcity: // Returns the specified player's cities
                break;
            default:
                move_status = Game::validate_move(command);
                if ((move_status == true) && (command != "done")){
                    Game::actions.push_front(command);
                }
        }

        
    }

    printf("Now performing actions...\n");

    while(!Game::actions.empty()){
        std::cout << "Runnning: " << Game::actions.back() << std::endl;
        Game::actions.pop_back();
    }

    printf("Round %d is now complete.\n\n", Game::round);

}

Game::ImmediateCommands Game::convertToImmediate(std::string command){
    if (command == "done") return ImmediateCommands::done;
    if (command == "undo") return ImmediateCommands::undo;
    if (command == "listmap") return ImmediateCommands::listmap;
    if (command == "listunit") return ImmediateCommands::listunit;
    if (command == "listcity") return ImmediateCommands::listcity;
    return ImmediateCommands::none;
}

bool Game::validate_move(std::string command){
    return true;
}

void Game::render() {
    /*
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    map->render(renderer);

    SDL_RenderPresent(renderer);
    */
}

void Game::clean() {
    //SDL_DestroyWindow(window);
    //SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
