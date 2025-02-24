#include "game.h"
// unit ids (1xx for team 1, 2xx for team 2)
// figure out how data is stored and shared between lua and C
// Shared pointers
Game::Game() : window(nullptr), renderer(nullptr), running(false), map(nullptr) {}

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
    Unit *testUnit = new Unit(0,0,10,1,0);
    map = new Map(20, 20);


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

    printf("Round %d begins...\n", Game::round);

    while(command != "done"){
        printf("Enter command: ");
        getline(std::cin, command);
        // validate move does nothing atm.
        bool move_status = Game::validate_move(command);
        if (command == "undo"){
            std::cout << "Reversing command: " << Game::actions.front() << std::endl;
            Game::actions.pop_front();
        } 
        else if ((move_status == true) && (command != "done")){
            Game::actions.push_front(command);
        }
    }

    printf("Now performing actions...\n");

    while(!Game::actions.empty()){
        std::cout << "Runnning: " << Game::actions.back() << std::endl;
        Game::actions.pop_back();
    }

    printf("Round %d is now complete.\n\n", Game::round);

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
