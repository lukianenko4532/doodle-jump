cmake_minimum_required(VERSION 3.16)
project(doodle_jump CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic)

find_package(Threads REQUIRED)

set(SOURCES
        src/main.cpp
)

add_executable(doodle_jump ${SOURCES})

target_link_libraries(doodle_jump PRIVATE Threads::Threads)