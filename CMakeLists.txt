
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_NAME "Myne")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-const-correctness") # Do not have the compile time for this, but good debug

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

add_library(${LIB_NAME}
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
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(BLEND2D_NO_JIT true) # No need for jit, I presume. Would require adding another subdir in /blend2d/.../?
add_subdirectory(include/blend2d)
add_subdirectory(include/SDL)
add_subdirectory(include/box2d)
add_subdirectory(include/SDL_image)

include_directories(include include/SDL_image/include)

target_link_libraries(${LIB_NAME} PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image

    box2d
    
    blend2d
    )

target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -pedantic -fsafe-buffer-usage-suggestions -O3 -march=native)
