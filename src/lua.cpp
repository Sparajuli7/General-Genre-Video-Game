#include <lua.hpp>
#include <iostream>
#include "lauxlib.h"
#include "lua.h"
#include "lua.hpp"
#include "map.h"
#include "unit.h"
#include "city.h"
#include "player.h"
#include "game.h"

namespace lua {

// Sets up an initial lua environment, loaded with the strategry library, along with a specified script file
lua_State* getInitialEnviron(const char script[]) {
    lua_State* L = luaL_newstate();

    loadStrategyLibrary(L);
    luaL_dofile(L, script);
    luaL_openlibs(L);

    // Ensure `think' function exists in provided script file
    if (lua_getglobal(L, "think") != LUA_TFUNCTION) {
        std::cerr << "Think function does not exist in " << script << "!" << std::endl;
        return nullptr;
    }

    return L;
}

/* Compute distance and path between two maptiles
 * Takes:
 *      int: UUID of from maptile
 *      int: UUID of to maptile
 * Returns:
 *      int: minimal distance between two maptiles
 *      table: list of uuids representing path between maptiles
 */
int maptilePath(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 2) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve maptile uuids from lua stack
    int a_uuid = luaL_checkinteger(L, 1);
    int b_uuid = luaL_checkinteger(L, 2);

    // Convert uuids to MapTiles
    MapTile* a = MapTile::uuidToMaptile(a_uuid);
    MapTile* b = MapTile::uuidToMaptile(b_uuid);

    // Compute distance and path
    auto [distance, path] = MapTile::path(a, b);

    // Push distance on to stack and return
    lua_pushinteger(L, distance);

    // Convert path vector to table
    lua_newtable(L);
    n = lua_gettop(L);
    for (int i = 0; i < path.size(); i++) {
        Uuid nodeUuid = path[i]->uuid;
        lua_pushinteger(L, nodeUuid);
        lua_rawseti(L, n, i + 1);
    }
    
    return 2;
}

/* Get table of maptile UUIDs neighboring a particular maptile
 * Takes:
 *      int: UUID of maptile
 * Returns:
 *      table: list of maptile UUIDs neighboring said maptile
 */
int maptileNeighbors(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve maptile uuid from lua stack
    int maptileId = luaL_checkinteger(L, 1);
    MapTile* maptile = MapTile::uuidToMaptile(maptileId);

    // Create 1-indexed table with a list of neighboring maptile uuids
    lua_newtable(L);
    n = lua_gettop(L);
    int i = 1;
    for (auto &neighbor: maptile->getNeighbors()) {
        Uuid neighborUuid = neighbor->uuid;
        lua_pushinteger(L, neighborUuid);
        lua_rawseti(L, n, i);
        i++;
    }

    return 1;
}

/* Get UUID of unit on a particular maptile
 * Takes:
 *      int: UUID of maptile
 * Returns:
 *      int: UUID of unit on maptile, 0 if no unit found
 */
int maptileGetUnit(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve maptile uuid from stack
    int maptileId = luaL_checkinteger(L, 1);
    // TODO: lookup maptile from uuid here
    MapTile* maptile = MapTile::uuidToMaptile(maptileId);

    // TODO: get unit
    int unitId = 0;
    lua_pushinteger(L, unitId);

    return 1;
}

/* Get UUID of city on a particular maptile
 * Takes:
 *      int: UUID of maptile
 * Returns:
 *      table: UUID of city on maptile, 0 if no city found
 */
int maptileGetCity(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve maptile uuid from stack
    int maptileId = luaL_checkinteger(L, 1);
    MapTile* maptile = MapTile::uuidToMaptile(maptileId);

    // TODO: Implement this
    int cityId = 0;
    lua_pushinteger(L, cityId);

    return 1;
}

/* Get table of unit UUIDs owned by a particular player
 * Takes:
 *      int: UUID of player
 * Returns:
 *      table: list of unit UUIDs owned by player
 */
int playerGetUnits(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve player uuid from stack
    int playerId = luaL_checkinteger(L, 1);
    Player* player = Player::uuidToPlayer(playerId);

    // Push table with a list of neighboring maptile uuids
    lua_newtable(L);
    n = lua_gettop(L);
    int i = 1;
    for (auto &unit: player->getUnits()) {
        lua_pushinteger(L, unit->getUUID());
        lua_rawseti(L, n, i);
        i++;
    }

    return 1;
}

/* Get table of city UUIDs owned by a particular player
 * Takes:
 *      int: UUID of player
 * Returns:
 *      table: list of city UUIDs owned by player
 */
 int playerGetCities(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve player uuid from stack
    int playerId = luaL_checkinteger(L, 1);
    Player* player = Player::uuidToPlayer(playerId);

    // Push table with a list of city uuids owned by player
    lua_newtable(L);
    n = lua_gettop(L);
    int i = 1;
    for (auto const &city: player->getCities()) {
        lua_pushinteger(L, city->getUUID());
        lua_rawseti(L, n, i);
        i++;
    }

    return 1;
}

/* Get UUID of the maptile a unit currently resides on
 * Takes:
 *      int: UUID of unit
 * Returns:
 *      int: UUID of unit's current maptile
 */
 int unitGetMaptile(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve player uuid from stack
    int unitId = luaL_checkinteger(L, 1);
    Unit* unit = Unit::uuidToUnit(unitId);

    // Push maptile uuid
    int maptileId = unit->getTileUUID();
    lua_pushinteger(L, maptileId);

    return 1;
}

/* Get UUID of the maptile a city currently resides on
 * Takes:
 *      int: UUID of city
 * Returns:
 *      int: UUID of city's current maptile
 */
 int cityGetMaptile(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Retrieve player uuid from stack
    int cityId = luaL_checkinteger(L, 1);
    City* city = City::uuidToCity(cityId);

    // Push maptile uuid
    int maptileId = city->getTileUUID();
    lua_pushinteger(L, maptileId);

    return 1;
}

/* Get table of player UUIDs
 * Takes:
 *      Nothing
 * Returns:
 *      table: list of player UUIDs
 */
int getPlayers(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 0) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Push table with a list of all player uuids
    lua_newtable(L);
    n = lua_gettop(L);
    int i = 1;
    for (auto const &[id, player]: Player::getPlayers()) {
        lua_pushinteger(L, id);
        lua_rawseti(L, n, i);
        i++;
    }

    return 1;
}

/* Get table of city UUIDs
 * Takes:
 *      Nothing
 * Returns:
 *      table: list of city UUIDs
 */
int getCities(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 0) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Push table with a list of all city uuids
    lua_newtable(L);
    n = lua_gettop(L);
    int i = 1;
    for (auto const &[id, city]: City::getCities()) {
        lua_pushinteger(L, id);
        lua_rawseti(L, n, i);
        i++;
    }

    return 1;
}

/* Get table of unit UUIDs
 * Takes:
 *      Nothing
 * Returns:
 *      table: list of unit UUIDs
 */
int getUnits(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 0) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Push table with a list of all unit uuids
    lua_newtable(L);
    n = lua_gettop(L);
    int i = 1;
    for (auto const &[id, unit]: Unit::getUnits()) {
        lua_pushinteger(L, id);
        lua_rawseti(L, n, i);
        i++;
    }

    return 1;
}

/* Commits a move to the action queue
 * Takes:
 *      table: movement enum
 * Returns:
 *      boolean: true if move is possible, false if move was rejected
 */
int makeMove(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 1) {
        luaL_error(L, "Incorrect number of arguments");
    }

    // Grab id from first index of table
    lua_pushinteger(L, 1);
    lua_gettable(L, 1);
    int id = luaL_checkinteger(L, -1);

    // Do the same with possible arguments, really it would be better to have this be variadic...
    lua_pushinteger(L, 2);
    lua_gettable(L, 1);
    int arg1 = luaL_checkinteger(L, -1);
    lua_pushinteger(L, 3);
    lua_gettable(L, 1);
    int arg2 = luaL_checkinteger(L, -1);

    //
    Game::validate_move({static_cast<GameCommandId>(id), arg1, arg2});

    return 1;
}

/* Undoes the most recent move that was committed to the action queue
 * Takes:
 *      Nothing
 * Returns:
 *      boolean: true if move was undone, false if action queue was empty
 */
int undoMove(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 0) {
        luaL_error(L, "Incorrect number of arguments");
    }

    return 1;
}

// C++ wrapper functions that will be marked as callable by Lua
static const struct luaL_Reg funcs[] = {
    {"maptilePath", maptilePath},
    {"maptileGetUnit", maptileGetUnit},
    {"maptileGetCity", maptileGetCity},
    {"playerGetUnits", playerGetUnits},
    {"playerGetCities", playerGetCities},
    {"unitGetMaptile", unitGetMaptile},
    {"cityGetMaptile", cityGetMaptile},
    {"getPlayers", getPlayers},
    {"getCities", getCities},
    {"getUnits", getUnits},
    {"makeMove", makeMove},
    {"undoMove", undoMove},
    {NULL, NULL}
};

// Update lua context to include `strategy' library
// TODO: make library nicer, e.g. strategy.maptile.getCity(), or strategy.maptile.city instead of strategy.maptileGetCity() 
void loadStrategyLibrary(lua_State* L) {
    // Regiseter C functions to a library table, then assign said table to global lua namespace
    luaL_newlib(L, funcs);
    lua_setglobal(L, "strategy");
}

// Calls a lua function called `think' with the provided player uuid
void runAI(lua_State* L, int uuid) {
    std::cout << "AI is thinking..." << std::endl;
    lua_getglobal(L, "think");
    lua_pushinteger(L, uuid);
    lua_call(L, 1, 0);
}
}