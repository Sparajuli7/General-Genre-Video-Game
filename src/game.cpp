#include "game.h"

Game::Game() : window(nullptr), renderer(nullptr), running(false), map(nullptr), L(nullptr) {}

Game::~Game() {
    // Currently this crashes the program
    delete map;
    delete &units;
    delete &cities;

}

void Game::init() {
    // Create SDL window and renderer
    if (renderOn){
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Strategy Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
        renderer = SDL_CreateRenderer(window, -1, 0);
    }    
    running = true;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    Game::round = 0;
    

    // Create map

    map = new Map(5, 5);

    std::uniform_int_distribution<> distr(0, map->size());


    // Create two cities.
    MapTile *randTile = map->findNode(distr(gen));
    cities.insert({101, new City(101, randTile)});

    randTile = map->findNode(distr(gen));
    cities.insert({201, new City(201, randTile)});

    // Create one unit on each generated city.
    for( auto itr = cities.begin(); itr != cities.end(); ++itr){
        Unit *tempUnit = itr->second->createUnit(units, 10, 1);
        units.insert({tempUnit->id, tempUnit});
        itr->second->unitCreatedThisTurn = false;
    }

}

// Doesn't really do anything at the moment, but will potentially be used for user input.
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
    bool move_status = false; // used to check if a game command worked.

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
            // TODO: Implement listmap and listunit
            case ImmediateCommands::listmap: // Returns the map and provides info on what units and cities are in each tile
                break;
            case ImmediateCommands::listunit: // Returns the specified player's units
                break;
            case ImmediateCommands::listcity: // Returns the specified player's cities
                listCity();
                break;
            case ImmediateCommands::help:
                std::cout << "\nGame Commands:\n"
                    "done: Completes the turn and applies all actions to game.\n"
                    "undo: Removes last action from queue.\n"
                    "help: Prints this command list.\n"
                    "quit: Will quit the game. Nothing is saved."
                    "listmap: Returns the map and coordinates of each tile.\n"
                    "listunit [player number]: Returns the specified player's units\n"
                    "listcity [player number]: Returns the specified player's cities\n"
                    "move [unit id xxx] [tile coordinate]: Attempts to move the unit to the specified map tile. Can only move one tile per turn.\n"
                    "attack [unit id xxx]: Attempts to attack an opposing player's unit if the specified unit is sharing a tile with one.\n"
                    "makeunit [city id xxx]: Attempts to create a unit on the specified city. Can only be done once per turn.\n\n";
                break;
            // Crashes the game but technically does quit the game.
            case ImmediateCommands::quit:
                Game::running = false;
                command = "done";
                break;
            // If no immediate commands are found, switch to checking commands to be queued.
            case ImmediateCommands::none:
                move_status = Game::validate_move(splitCommand); 
                if ((move_status == true) && (splitCommand[0] != "done")){
                    Game::actions.push_front(command);
                }
        }

        
    }

    printf("Now performing actions...\n");

    // TODO: Immplement queue to actually play back commands.
    while(!Game::actions.empty()){
        std::cout << "Runnning: " << Game::actions.back() << std::endl;
        Game::actions.pop_back();
    }

    printf("Round %d is now complete.\n\n", Game::round);

}

// Enum converter
Game::ImmediateCommands Game::convertToImmediate(std::string command){
    if (command == "done") return ImmediateCommands::done;
    if (command == "undo") return ImmediateCommands::undo;
    if (command == "listmap") return ImmediateCommands::listmap;
    if (command == "listunit") return ImmediateCommands::listunit;
    if (command == "listcity") return ImmediateCommands::listcity;
    if (command == "help") return ImmediateCommands::help;
    if (command == "quit") return ImmediateCommands::quit;
    return ImmediateCommands::none;
}

// Simply lists the cities with their ID and location on the map.
void Game::listCity(){
    for (auto itr = cities.begin(); itr != cities.end(); ++itr){
        std::cout << "City ID: " << itr->second->getID() << "  City Location: " << itr->second->getTileID() << std::endl;
    }
}

// Enum converter
Game::GameCommands Game::convertToGame(std::string command){
    if (command == "move") return GameCommands::move;
    if (command == "attack") return GameCommands::attack;
    if (command == "makeunit") return GameCommands::makeunit;
    return GameCommands::unknown;
}

/* Game actions that are saved to the deque before being run in the main game to support undoing.*/
// Currently implemented code will automatically run for testing
bool Game::validate_move(std::vector<std::string> command){
    switch (convertToGame(command[0]))
    {
    // TODO: Will be implemented soon.
    case GameCommands::move:
        return true;
    // TODO: Code made by Knox already, to be implemented and switched to primarly work within unit.cpp
    case GameCommands::attack:
        units.at(std::stoi(command[1]))->attack(*units.at(std::stoi(command[2])));
        std::cout << "Current Health of Unit #" << units.at(std::stoi(command[2]))->id << " is now: " << units.at(std::stoi(command[2]))->getHealth() << std::endl;
        return true;
        //code to delete a unit from the map.
        //units.erase(target_id);
        break;
    // TODO: Implement makeunit code
    case GameCommands::makeunit:
        return true;
        break;
    case GameCommands::unknown:
        std::cout << "Unknown command, enter \"help\" for list of commands.\n";
        return false;
    }
    return false;
}

// TODO: Render the rest of the game objects like cities
void Game::render() {

    if (renderOn){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        map->render(renderer);
        // Renders every unit in the game.
        for (auto itr = units.begin(); itr != units.end(); ++itr){
            itr->second->render(renderer);
        }

        SDL_RenderPresent(renderer);
    }
}

// Used to remove the SDL renderer and window
void Game::clean() {
    if(renderOn){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }
    SDL_Quit();
}
