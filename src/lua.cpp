#include "lua.hpp"
#include "map.h"

namespace lua {

// Sets up an initial lua environment, loaded with the strategry library, along with a specified script file
// TODO: add error checking on script load, check for `think' function
lua_State* getInitialEnviron(const char script[]) {
    lua_State* L = luaL_newstate();

    loadStrategyLibrary(L);
    luaL_dofile(L, script);

    return L;
}

/* Compute distance between two maptiles
 * Takes:
 *      int: UUID of from maptile
 *      int: UUID of to maptile
 * Returns:
 *      int: minimal distance between two maptiles
 */
int maptileDistance(lua_State* L) {
    // Verify number of input params
    int n = lua_gettop(L);
    if (n != 2) {
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

    // Retrieve maptile uuids from lua stack
    int from_uuid = luaL_checkinteger(L, 1);
    int to_uuid = luaL_checkinteger(L, 2);

    // TODO: insert call to MapTile.distance() here
    int distance = 0;

    // Push distance on to stack and return
    lua_pushinteger(L, distance);
 
    return 1;
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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

    // Retrieve maptile uuid from lua stack
    int maptileId = luaL_checkinteger(L, 1);
    // TODO: lookup maptile from uuid here
    MapTile* maptile = nullptr;

    // Create 1-indexed table with a list of neighboring maptile uuids
    lua_newtable(L);
    int i = 1;
    for (auto &neighbor: maptile->neighbors) {
        int neighborId = 0;
        lua_pushinteger(L, neighborId);
        lua_rawseti(L, i, 1);
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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

    // Retrieve maptile uuid from stack
    int maptileId = luaL_checkinteger(L, 1);
    // TODO: lookup maptile from uuid here
    MapTile* maptile = nullptr;

    // TODO: get unit
    int unitId = 0;
    lua_pushinteger(L, unitId)

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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

    // Retrieve maptile uuid from stack
    int maptileId = luaL_checkinteger(L, 1);
    // TODO: lookup maptile from uuid here
    MapTile* maptile = nullptr;

    // TODO: get unit
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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

    // Retrieve player uuid from stack
    int playerId = luaL_checkinteger(L, 1);
    Player* player = nullptr;

    // Create 1-indexed table with a list of neighboring maptile uuids
    lua_newtable(L);
    int i = 1;
    for (auto &unit: player->units) {
        int unitId = 0;
        lua_pushinteger(L, unitId);
        lua_rawseti(L, i, 1);
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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

    // Retrieve player uuid from stack
    int playerId = luaL_checkinteger(L, 1);
    Player* player = nullptr;

    // Push 1-indexed table with a list of city uuids owned by player
    lua_newtable(L);
    int i = 1;
    for (auto &city: player->cities) {
        int cityId = 0;
        lua_pushinteger(L, cityId);
        lua_rawseti(L, i, 1);
    }

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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

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
        lua_pushliteral(L, "incorrect number of arguments");
        lua_error(L);
    }

    return 1;
}

// C++ wrapper functions that will be marked as callable by Lua
static const struct luaL_Reg funcs[] = {
    {"maptileDistance", maptileDistance},
    {"maptileGetUnit", maptileGetUnit},
    {"maptileGetCity", maptileGetCity},
    {"playerGetUnits", playerGetUnits},
    {"playerGetCities", playerGetCities},
    {"getPlayers", getPlayers},
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
    lua_getglobal(L, "think");
    lua_pushinteger(L, uuid);
    lua_call(L, 0, 0);
}
}