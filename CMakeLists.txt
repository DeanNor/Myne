
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

file(GLOB imgui-docking "include/imgui-docking/*.cpp") # TODO remove, see below

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_executable(${PROJECT_NAME} main.cpp
    .cpp/drawobj.cpp
    .cpp/blendobj.cpp
    .cpp/game.cpp
    .cpp/object.cpp
    .cpp/pos.cpp
    .cpp/process.cpp
    .cpp/display.cpp
    .cpp/collobj.cpp
    .cpp/tfm.cpp
    .cpp/joint.cpp
    .cpp/ref.cpp
    .cpp/SDL3.cpp
    .cpp/hull.cpp
    .cpp/factory.cpp
    .cpp/loader.cpp
    .cpp/saver.cpp
    .cpp/tube.cpp

    ${imgui-docking}
    "include/imgui-docking/backends/imgui_impl_sdl3.cpp"
    "include/imgui-docking/backends/imgui_impl_sdlrenderer3.cpp"
    "include/imgui-docking/misc/cpp/imgui_stdlib.cpp"
    
    #TODO ^ separate the imgui version to use the compiled version of this library instead of compiling with it
    )

include_directories(.hpp)

include_directories(include)

include_directories(include/imgui-docking) # TODO remove, see above
include_directories(include/imgui-docking/backends) #TODO remove, see above

find_library(blend2d NAMES blend2d PATHS /usr/local/lib64/) # Find shared blend2d library

find_library(SDL3_image NAMES SDL3_image PATHS /usr/local/lib64/) # Find shared SDL3_image library

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3

    box2d
    
    ${blend2d}
    
    ${SDL3_image}
    )

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic)