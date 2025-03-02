#include "game.h"

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
        std::string s;
        std::stringstream ss(command);
        std::vector<std::string> splitCommand;
        // Splits Command by spaces
        while (getline(ss, s, ' ')) {
            // store token string in the vector
            splitCommand.push_back(s);
        }

        switch (convertToImmediate(splitCommand[0])) {
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
            case ImmediateCommands::help:
                std::cout << "\nGame Commands:\n"
                    "done: Completes the turn and applies all actions to game.\n"
                    "undo: Removes last action from queue.\n"
                    "help: Prints this command list.\n"
                    "listmap: Returns the map and coordinates of each tile.\n"
                    "listunit [player number]: Returns the specified player's units\n"
                    "listcity [player number]: Returns the specified player's cities\n"
                    "move [unit id xxx] [tile coordinate]: Attempts to move the unit to the specified map tile. Can only move one tile per turn.\n"
                    "attack [unit id xxx]: Attempts to attack an opposing player's unit if the specified unit is sharing a tile with one.\n"
                    "makeunit [city id xxx]: Attempts to create a unit on the specified city. Can only be done once per turn.\n\n";
                break;
            case ImmediateCommands::none:
                move_status = Game::validate_move(splitCommand); 
                if ((move_status == true) && (splitCommand[0] != "done")){
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
    if (command == "help") return ImmediateCommands::help;
    return ImmediateCommands::none;
}

Game::GameCommands Game::convertToGame(std::string command){
    if (command == "move") return GameCommands::move;
    if (command == "attack") return GameCommands::attack;
    if (command == "makeunit") return GameCommands::makeunit;
    return GameCommands::unknown;
}

bool Game::validate_move(std::vector<std::string> command){
    switch (convertToGame(command[0]))
    {
    case GameCommands::move:
        return true;
    case GameCommands::attack:
        return true;
        break;
    case GameCommands::makeunit:
        return true;
        break;
    case GameCommands::unknown:
        std::cout << "Unknown command, enter \"help\" for list of commands.\n";
        return false;
    }
    return false;
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
