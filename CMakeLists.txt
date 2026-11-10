cmake_minimum_required(VERSION 3.16)
project(doodle_jump CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic)

find_package(Threads REQUIRED)

set(SOURCES
        src/main.cpp
        src/TerminalManager.cpp
        src/TerminalManager.hpp
        src/Player.cpp
        src/Player.hpp
        src/Platform.cpp
        src/Platform.hpp
        src/Board.cpp
        src/Board.hpp
        src/GameEngine.cpp
        src/GameEngine.hpp
        src/ArgParser.cpp
        src/ArgParser.hpp
)

add_executable(doodle_jump ${SOURCES})

target_link_libraries(doodle_jump PRIVATE Threads::Threads)