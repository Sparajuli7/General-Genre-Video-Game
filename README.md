# Video Game Artificial Intelligence

By The Explorers

The Explorers: 
Dylan Andrews, Kieran Firkin, Knox Brown, Shreyash Parajuli, Sharifullah Abed

This game is our capstone project for the 2024-2025 school year of our computer science degree.
It is a simple strategy game that can be controlled either a human or an AI player. 
The AI is controlled by a lua script that can gather info and perform actions using functions provided by the C++ side of the program.
It features basic graphics to help with user understanding.

## Build Instructions

### Dependencies 

Git

CMake

GCC

G++

SDL2:

- Ubuntu: `libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev` 

- Fedora: `SDL2-devel SDL2_image-devel SDL2_ttf-devel`

Lua

- Ubuntu: `liblua5.4-dev`

- Fedora: `lua-devel`

The program uses Swansea-q3pd.ttf as a font, which can be found at https://www.fontspace.com/swansea-font-f5873 

### Compilation

Run build commands:

`cmake CMakeLists.txt`

`make`

## Usage

Run `./StratagyGame`

Once game begins, `help` will show game commands

## Lua Scripting

Taken from the User Guide

User provided Lua scripts should be named `ai.lua`, and placed in a folder labelled `lua`. When loaded, the script should be able to access a module called “strategy”. This module provides bindings for various C++ functions to access and modify the current game state. The following subsections will detail the operation of said functions. Lua is not a strongly typed language, but function signatures are provided as though it was.

1 	maptilePath

`int distance, table path maptilePath(int source_uuid, int target_uuid)`

Computes the minimal distance and path between two maptiles. The two input maptiles are passed using their uuids, and a table of maptile uuids representing the minimal path is returned.

2 	maptileNeighbors

`table neighbor_uuids maptileNeighbors(int maptile_uuid)`

Gets the neighbors of a particular maptile. Takes a maptile uuid, and returns a table of neighbor maptile uuids. 

3 	playerGetUnits

`table unit_uuids playerGetUnits(int player_uuid)`

Gets the units associated with a particular player. Takes a player uuid, and returns a table of unit uuids.

4 	playerGetCities

`table city_uuids playerGetCities(int player_uuid)`

Gets the cities associated with a particular player. Takes a player uuid, and returns a table of city uuids.

5 	unitGetMaptile

`int maptile_uuid unitGetMaptile(int unit_uuid)`

Gets the maptile a unit currently resides on. Takes a unit uuid, and returns a maptile uuid.

6 	cityGetMaptile

`int maptile_uuid cityGetMaptile(int city_uuid)`

Gets the maptile a city currently resides on. Takes a city uuid, and returns a maptile uuid.

7 	getPlayers

`table player_uuids getPlayers()`

Gets a list of all players in the game. Returns a table of player uuids.

8 	getCities

`table city_uuids getCities()`

Gets a list of all cities in the game. Returns a table of city uuids.

9 	getUnits

`table unit_uuids getUnits()`

Gets a list of all units in the game. Returns a table of unit uuids.

10 	makeMove

`bool validated makeMove(table move)`

Attempts to make a move. Takes a three element table representing the move packet, where the first element represents the action id. A value of 1 indicates that it is a movement action, and the following table elements represent the uuid of a unit to move, followed by the uuid of the adjacent maptile to move to. A value of 2 indicates that it is an attack action, and the following table elements represent the uuid of the unit making an attack, followed by the uuid of the unit or city being attacked. A value of 3 indicates that it is a create unit action, and the following table element represents the uuid of the city to create a unit. Returns a boolean representing whether the action succeeded.

## Bugs and Future Work

1. There appears to be a bug where the game will crash after an AI captures a city or defeats an enemy. It seems that some part of memory somewhere is not properly cleaned before re-use and causes some bad values.
Error:
`terminate called after throwing an instance of 'std::out_of_range'`
` what():  map::at`
2. The map is currently static and does not change outside of an odd connection of MapTiles.
3. There is no way to configure the game with different parameters. One way that was discussed by our team and the client was using JSON to store settings. This is likely best done using the json library for C++. https://github.com/nlohmann/json 
4. The graphical interface made in SDL2 is rather barebones, a UI manager that converts keyboard controls to commands run by the game would allow for a potentially simple user interface.