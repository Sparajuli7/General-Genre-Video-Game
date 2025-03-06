#pragma once

#include <lua5.4/lua.hpp>

namespace lua {
    lua_State* getInitialEnviron(const char script[]);
    void loadStrategyLibrary(lua_State* L);
    void runAI(lua_State* L);
}