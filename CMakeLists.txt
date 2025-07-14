
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

find_package(LLVM REQUIRED CONFIG) # TODO Not use sys installs for LLVM and CLANG despite that they suck to include also TODO remove as imgui
find_package(Clang REQUIRED CONFIG)

file(GLOB imgui-docking "include/imgui-docking/*.cpp") # TODO remove, see below

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

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

include_directories(include/imgui-docking) # TODO remove, see above
include_directories(include/imgui-docking/backends) #TODO remove, see above

target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS}) # TODO remove, same as imgui
target_link_libraries(${PROJECT_NAME} PRIVATE LLVM clang-cpp)

set(BLEND2D_NO_JIT true) # No need for jit, I presume. Would require adding another subdir in /blend2d/.../?
add_subdirectory(include/blend2d EXCLUDE_FROM_ALL)
add_subdirectory(include/SDL EXCLUDE_FROM_ALL)
add_subdirectory(include/box2d EXCLUDE_FROM_ALL)
add_subdirectory(include/SDL_image EXCLUDE_FROM_ALL)

include_directories(include include/SDL_image/include)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image

    box2d
    
    blend2d
    )

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic)
