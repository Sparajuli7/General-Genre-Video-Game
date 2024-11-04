cmake_minimum_required(VERSION 3.10)
project(StrategyGame)

# Add C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find SDL2 and Lua
find_package(SDL2 REQUIRED)
find_package(Lua REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})

# Add executable
add_executable(StrategyGame src/main.cpp src/game.cpp src/map.cpp src/unit.cpp)

# Link SDL2 and Lua libraries
target_link_libraries(StrategyGame ${SDL2_LIBRARIES} ${LUA_LIBRARIES})
