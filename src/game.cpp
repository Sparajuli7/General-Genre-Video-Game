#include "game.h"
#include "lua.h"
#include <string>
// This is hacky as hellllll, just turning it into a pseudo singleton
// Also I have no idea why this doesn't work as a static class member, so it's going as a static global instead, some sort of link error
// Whatever.
static Game* singleton;

Game::Game() : window(nullptr), renderer(nullptr), running(false), map(nullptr) {
    singleton = this;
}

Game::~Game() {
    delete map;

}

// Runs once to set up the game
void Game::init() {
    // Create SDL window and renderer
    if (renderOn){
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
        font = TTF_OpenFont("Swansea-q3pd.ttf", 18); // https://www.fontspace.com/swansea-font-f5873 
        if (!font) {
            std::cout << "Error: Font missing, game will likely crash." << std::endl;
        }
        window = SDL_CreateWindow("Strategy Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
        renderer = SDL_CreateRenderer(window, -1, 0);
    }    
    running = true;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    Game::round = -1;
    
    // The boolean will determine if the player is a bot.
    Player* player1 = Player::makePlayer(false);
    Player* player2 = Player::makePlayer(true);


    // Create map

    map = new Map(5, 5);

    std::uniform_int_distribution<> distr(0, map->size());

    // Create two cities.
    //MapTile *randTile = map->findNode(distr(gen));

    City::makeCity(MapTile::uuidToMaptile(MapTile::getMapTiles().begin()->first), player1);

    City::makeCity(MapTile::uuidToMaptile(MapTile::getMapTiles().end()->first), player2);

    // Create one unit on each generated city.
    for( auto const itr : City::getCities()){
        City::createUnit(itr.second->getUUID(), 10, 1, itr.second->getOwner());
        itr.second->unitCreatedThisTurn = false;
    }

}

// Checks for victory condition or if the window is closed
void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    Game::checkVictoryCondition();

    if (event.type == SDL_QUIT) {
        running = false;
    }
}

void Game::update() {

    Game::round++;
    currentPlayer = Player::getPlayers().find(round%2)->second;
    // Resets limited use actions 
    for (auto itr : City::getCities()){
        itr.second->unitCreatedThisTurn = false;
    }
    for (auto itr : Unit::getUnits()){
        itr.second->moved = false;
    }

    printf("Round %d begins...\n", Game::round+1);
    std::cout << "It is player " << currentPlayer->getUUID() << "'s turn." << std::endl;
    std::deque<GameCommand> actions = currentPlayer->takeAction(); // Will determine if AI will be run or requestInput will be run.


    printf("Now performing actions...\n");

    // TODO: Immplement queue to actually play back commands.
    while(!actions.empty()){
        actions.pop_back();
    }

    printf("Round %d is now complete.\n\n", Game::round+1);

}

// Function used to recieved human player input when it's their turn
void Game::requestInput() {
    std::string command = "";
    bool move_status = false; // Used to check if a game command worked.
    std::deque<std::string> actions;

    while (command != "done") {
        getline(std::cin, command);
        std::string s;
        std::stringstream ss(command);
        std::vector<std::string> splitCommand;
        // Splits Command by spaces
        while (getline(ss, s, ' ')) {
            // Store token string in the vector
            splitCommand.push_back(s);
        }
        // Convert first string in splitCommand to a number that can be used for a switch case
        switch (convertToImmediate(splitCommand[0])) {
            case ImmediateCommands::done:
                command = "done";
                break;
            case ImmediateCommands::undo: // Not used, like other command queue items
                std::cout << "Reversing command: " << actions.front() << std::endl;
                actions.pop_front();
                break;
            case ImmediateCommands::listmap: // Unused, could be used to provide map connections to a player without GUI 
                break;
            case ImmediateCommands::listunit: // Returns the specified player's units
                singleton->listUnit();
                break;
            case ImmediateCommands::listcity: // Returns the specified player's cities
                singleton->listCity();
                break;
            case ImmediateCommands::help:
                std::cout << "\nGame Commands:\n"
                    "done: Completes the turn and applies all actions to game.\n"
                    "undo: Removes last action from queue.\n"
                    "help: Prints this command list.\n"
                    "quit: Will quit the game. Nothing is saved.\n"
                    "listmap: Returns the map and coordinates of each tile.\n"
                    "listunit [player number]: Returns the specified player's units\n"
                    "listcity [player number]: Returns the specified player's cities\n"
                    "move [unit id xxx] [tile coordinate]: Attempts to move the unit to the specified map tile. Can only move one tile per turn.\n"
                    "attack [unit id xxx]: Attempts to attack an opposing player's unit if the specified unit is sharing a tile with one.\n"
                    "makeunit [city id xxx]: Attempts to create a unit on the specified city. Can only be done once per turn.\n\n";
                break;
            // Quits the game immediately, seems to be unstable
            case ImmediateCommands::quit:
                singleton->running = false;
                command = "done";
                break;
            // If no immediate commands are found, switch to game commands.
            case ImmediateCommands::none:
                move_status = singleton->validate_move(Game::convertToGame(command)); 
                if ((move_status == true) && (splitCommand[0] != "done")){
                    actions.push_front(command);
                }
        }
    }
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

// Lists the cities with their ID and location on the map.
void Game::listCity(){
    for (auto const itr : City::getCities() ){
        std::cout << "City ID: " << itr.second->getUUID() << "  City Location: " << itr.second->getTileUUID() << std::endl;
    }
}

// Lists the units with their ID, location on the map, and their health and attack power
void Game::listUnit(){
    for (auto itr : Unit::getUnits()){
        std::cout << "Unit ID: " << itr.second->getUUID() << "  Location: " << itr.second->getTileUUID() << "  Health & Attack: " <<  itr.second->getHealth() << ", " << itr.second->getAttack() << std::endl;
    }
}

// Enum converter
GameCommand Game::convertToGame(std::string commandString){
    // Splits Command by spaces
    std::string s;
    std::stringstream ss(commandString);
    std::vector<std::string> splitCommand;
    while (getline(ss, s, ' ')) {
        // store token string in the vector
        splitCommand.push_back(s);
    }

    GameCommand command;
    command.id = GameCommandId::Unknown;
    if (splitCommand[0] == "move") {
        if (splitCommand.size() != 3){
            std::cout << "Error: Missing arguments.\n";
            return command;
        }
        command.id = GameCommandId::Move;
        command.move.unit = std::stoi(splitCommand[1]);
        command.move.mapTile = std::stoi(splitCommand[2]);
    } else if (splitCommand[0] == "attack") {
        if (splitCommand.size() != 3){
            std::cout << "Error: Missing arguments.\n";
            return command;
        }
        command.id = GameCommandId::Attack;
        command.attack.unit = std::stoi(splitCommand[1]);
        command.attack.target = std::stoi(splitCommand[2]);
    } else if (splitCommand.front() == "makeunit") {
        if (splitCommand.size() != 2){
            std::cout << "Error: Missing arguments.\n";
            return command;
        }
        command.id = GameCommandId::Makeunit;
        command.makeUnit.city = std::stoi(splitCommand[1]);
    }

    return command;
}

// Currently implemented code will automatically run as deque for human players is not implemented
bool Game::validate_move(GameCommand command){
    switch (command.id)
    {
    case GameCommandId::Move:
        return Unit::move(command.move.unit, command.move.mapTile, *(singleton->map), singleton->currentPlayer);
        break;
    case GameCommandId::Attack:
        return Unit::attackUnit(command.attack.unit, command.attack.target, singleton->currentPlayer);
        break;
    case GameCommandId::Makeunit:
        return City::createUnit(command.makeUnit.city, 10, 10, singleton->currentPlayer);
        break;
    case GameCommandId::Unknown:
        std::cout << "Unknown command, enter \"help\" for list of commands.\n";
        return false;
    }
    return false;
}

void Game::checkVictoryCondition() {
    int alivePlayers = 0;
    Player* potentialWinner = nullptr;

    for (auto itr : Player::getPlayers()) {
        if (!itr.second->isDefeated()) {
            alivePlayers++;
            potentialWinner = itr.second;
        }
    }

    if (alivePlayers == 1) {
        std::cout << "Player " << potentialWinner->getUUID() << " has won the game!\n";
        running = false;
    }
}

void Game::render() {
    try {
        if (renderOn){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            map->render(renderer, font);
            Unit::renderAll(renderer);
            City::renderAll(renderer);
            SDL_RenderPresent(renderer);
        }
    }
    catch (int e){
        std::cout << "Render Error! #" << e << std::endl;
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

