
cmake_minimum_required(VERSION 3.31.0)

project("TEST")
set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_INSTALL_LIBDIR ${PROJECT_BINARY_DIR})
set(CMAKE_INSTALL_BINDIR ${PROJECT_BINARY_DIR})

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-const-correctness") # Do not have the compile time for this, but good debug

set(LIB_NAME "Myne")

# set(IMGUI_PATH include/imgui-docking)
# file(GLOB IMGUI_GLOB
#     ${IMGUI_PATH}/imgui.cpp
#     ${IMGUI_PATH}/imgui_draw.cpp
#     ${IMGUI_PATH}/imgui_tables.cpp
#     ${IMGUI_PATH}/imgui_widgets.cpp

#     ${IMGUI_PATH}/backends/imgui_impl_sdl3.cpp
#     ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer3.cpp
    
#     ${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
# )

# add_library(ImGui STATIC ${IMGUI_GLOB})
# include_directories(ImGui PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends)

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
    .cpp/hash.cpp

    # ast/ts_tool.c
    # ast/ast_stuff.cpp
    # editorstuff.cpp
    # editorobj.cpp
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(BLEND2D_NO_JIT true) # No need for jit, I presume. Would require adding another subdir in /blend2d/.../?
add_subdirectory(include/blend2d)
add_subdirectory(include/SDL)
add_subdirectory(include/box2d)
add_subdirectory(include/SDL_image)
# add_subdirectory(include/tree-sitter)
# add_subdirectory(include/tree-sitter-cpp)
add_subdirectory(include/json)

include_directories(include include/SDL_image/include include/SDL/include include/box2d/include/ include/blend2d/blend2d
    ${IMGUI_PATH}
)

find_package(box2d QUIET)
if (!box2d_FOUND)
    add_dependencies(${LIB_NAME} box2d)
endif()

# find_package(nlohmann_json QUIET)
# if (!nlohmann_json_FOUND)
#     add_dependencies(${LIB_NAME} nlohmann_json)
# endif()

target_link_libraries(${LIB_NAME}
    PRIVATE SDL3::SDL3
    PRIVATE SDL3_image::SDL3_image

    PRIVATE box2d
    
    PRIVATE blend2d

    # PRIVATE ImGui

    # PRIVATE tree-sitter

    PRIVATE nlohmann_json::nlohmann_json
)

# target_compile_definitions(${LIB_NAME} PUBLIC EDITOR) # Sets defines for editor
target_compile_definitions(${LIB_NAME} PUBLIC NO_PHYS) # Sets defines for removing physics checks ever

# set_target_properties(${LIB_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
# target_compile_options(${LIB_NAME} PRIVATE -Os -fdata-sections -ffunction-sections)
# target_link_options(${LIB_NAME} PRIVATE -Wl,--gc-sections -s)

target_compile_options(${LIB_NAME} PUBLIC -pedantic -O3 -march=native)

if (win32)
    add_custom_command(TARGET ${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${LIB_NAME}> $<TARGET_FILE_DIR:${LIB_NAME}>
        COMMAND_EXPAND_LISTS
    )

    target_compile_options(${LIB_NAME} PUBLIC -pedantic)
endif()
