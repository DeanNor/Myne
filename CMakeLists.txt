
cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(${PROJECT_NAME} main.cpp
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

add_subdirectory(SDL3_gfx Inc-Builds)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    box2d)