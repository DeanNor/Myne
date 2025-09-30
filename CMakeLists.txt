
cmake_minimum_required(VERSION 3.21)

project("TEST")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_INSTALL_LIBDIR ${PROJECT_BINARY_DIR})
set(CMAKE_INSTALL_BINDIR ${PROJECT_BINARY_DIR})

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-const-correctness") # Do not have the compile time for this, but good debug

set(LIB_NAME "Myne")

set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui-docking)
file(GLOB IMGUI_GLOB
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp

    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/imgui_demo.cpp

    ${IMGUI_PATH}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer3.cpp
)
add_library(ImGui STATIC ${IMGUI_GLOB})
target_include_directories(ImGui PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends)

add_executable(${LIB_NAME}
    main.cpp

    .cpp/drawobj.cpp
    .cpp/drawtarget.cpp
    .cpp/blendobj.cpp
    .cpp/game.cpp
    .cpp/object.cpp
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

    ast/ts_tool.c
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(BLEND2D_NO_JIT true) # No need for jit, I presume. Would require adding another subdir in /blend2d/.../?
add_subdirectory(include/blend2d)
add_subdirectory(include/SDL)
add_subdirectory(include/box2d)
add_subdirectory(include/SDL_image)
add_subdirectory(include/tree-sitter)
add_subdirectory(include/tree-sitter-cpp)

include_directories(include include/SDL_image/include include/box2d/include include/tree-sitter/lib/include/)

find_package(box2d QUIET)
if (!box2d_FOUND)
    add_dependencies(box2d)
endif()

target_link_libraries(${LIB_NAME}
    PUBLIC SDL3::SDL3
    PUBLIC SDL3_image::SDL3_image

    PUBLIC box2d
    
    PUBLIC blend2d

    PUBLIC ImGui

    PUBLIC tree-sitter
)

if (!win32)
    target_compile_options(${LIB_NAME} PUBLIC -pedantic)# -O3 -march=native)
endif ()

if (win32)
    add_custom_command(TARGET ${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${LIB_NAME}> $<TARGET_FILE_DIR:${LIB_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()