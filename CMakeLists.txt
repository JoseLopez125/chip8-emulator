cmake_minimum_required(VERSION 3.31)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 20)

# 1. Find the SDL2 package on your system
find_package(SDL2 REQUIRED)

# 2. Add the executable (your main.cpp)
add_executable(chip8_emulator main.cpp)

# 3. Include SDL2 headers so #include <SDL2/SDL.h> works
target_include_directories(chip8_emulator PRIVATE ${SDL2_INCLUDE_DIRS})

# 4. Link the SDL2 library to your executable
target_link_libraries(chip8_emulator PRIVATE ${SDL2_LIBRARIES})