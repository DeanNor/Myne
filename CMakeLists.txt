
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_NAME "Myne")

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-const-correctness") # Do not have the compile time for this, but good debug

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

include_directories(include include/SDL_image/include include/box2d/include)

target_link_libraries(${LIB_NAME}
    PUBLIC SDL3::SDL3
    PUBLIC SDL3_image::SDL3_image

    PUBLIC box2d
    
    PUBLIC blend2d
)

if (win32)
    add_custom_command(TARGET ${LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${LIB_NAME}> $<TARGET_RUNTIME_DLLS:${LIB_NAME}>
    COMMAND_EXPAND_LISTS
    )
endif ()