cmake_minimum_required(VERSION 3.10)
project(Game VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Game main.cpp
    .cpp/drawobj.cpp
    .cpp/game.cpp
    .cpp/object.cpp
    .cpp/pos.cpp
    .cpp/process.cpp
    .cpp/window.cpp
    .cpp/collobj.cpp
    .cpp/tfm.cpp
    )

include_directories(.h)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(${PROJECT_NAME} PUBLIC
    allegro
    allegro_primitives
    allegro_image
    allegro_audio

    box2d
    )

