cmake_minimum_required(VERSION 3.10)
project(StrategyGame)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(SDL2 REQUIRED)
find_package(Lua REQUIRED)

# For SDL2_ttf and SDL2_image, we'll use pkg-config as it's more reliable on Linux
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

# Add executable
add_executable(StrategyGame src/main.cpp src/game.cpp src/map.cpp src/unit.cpp src/city.cpp src/player.cpp src/lua.cpp)

# Link libraries
target_link_libraries(StrategyGame
    ${SDL2_LIBRARIES}
    ${LUA_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

# Add linker flags if needed (sometimes required on Linux)
target_link_options(StrategyGame PRIVATE
    -lSDL2_ttf
    -lSDL2_image
)