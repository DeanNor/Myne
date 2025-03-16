
cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(Game main.cpp
    .cpp/drawobj.cpp
    .cpp/game.cpp
    .cpp/object.cpp
    .cpp/pos.cpp
    .cpp/process.cpp
    .cpp/display.cpp
    .cpp/collobj.cpp
    .cpp/tfm.cpp
    .cpp/rad.cpp
    )

include_directories(.h)

target_link_libraries(Game PUBLIC
    SDL3::SDL3
    box2d)