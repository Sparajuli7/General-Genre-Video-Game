#pragma once

#include <lua.hpp>

namespace lua {
    lua_State* getInitialEnviron(const char script[]);
    void loadStrategyLibrary(lua_State* L);
    void runAI(lua_State* L, int playerId);
}