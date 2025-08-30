
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Construct the project name with processor information
project(Game C CXX)

set(EXE_NAME ${PROJECT_NAME}.${CMAKE_SYSTEM_NAME}.${CMAKE_SYSTEM_PROCESSOR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-const-correctness") # Do not have the compile time for this, but good debug

file(GLOB imgui-docking "include/imgui-docking/*.cpp") # TODO remove, see below

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

add_executable(${EXE_NAME} main.cpp
    .cpp/drawobj.cpp
    .cpp/blendobj.cpp
    .cpp/game.cpp
    .cpp/object.cpp
    .cpp/pos.cpp
    .cpp/process.cpp
    .cpp/display.cpp
    .cpp/collobj.cpp
    .cpp/tfm.cpp
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

    # TODO separate ast editor build
    ast/ts_tool.c
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(include/imgui-docking) # TODO remove, see above
include_directories(include/imgui-docking/backends) #TODO remove, see above

set(BLEND2D_NO_JIT true) # No need for jit, I presume. Would require adding another subdir in /blend2d/.../?
add_subdirectory(include/blend2d EXCLUDE_FROM_ALL)
add_subdirectory(include/SDL EXCLUDE_FROM_ALL)
add_subdirectory(include/box2d EXCLUDE_FROM_ALL)
add_subdirectory(include/SDL_image EXCLUDE_FROM_ALL)

include_directories(include include/SDL_image/include)

# TODO KILL \*/
add_subdirectory(include/tree-sitter/lib EXCLUDE_FROM_ALL)

include_directories(include include/tree-sitter/lib/include)

target_link_libraries(${EXE_NAME} PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image

    box2d
    
    blend2d

    # TODO KILL \/
    tree-sitter
    )

target_compile_options(${EXE_NAME} PUBLIC -Wall -Wextra -pedantic -fsafe-buffer-usage-suggestions -O3 -march=native)
