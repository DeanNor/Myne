
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(${PROJECT_NAME} main.cpp
    .cpp/drawobj.cpp
    .cpp/blendobj.cpp
    .cpp/game.cpp
    .cpp/object.cpp
    .cpp/pos.cpp
    .cpp/process.cpp
    .cpp/display.cpp
    .cpp/collobj.cpp
    .cpp/rad.cpp
    .cpp/tfm.cpp
    .cpp/joint.cpp
    .cpp/ref.cpp
    )

include_directories(.hpp)

include_directories(Include)

find_library(blend2d NAMES blend2d PATHS /usr/local/lib64/) # Find shared blend2d library

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3

    box2d
    
    ${blend2d}
    )
