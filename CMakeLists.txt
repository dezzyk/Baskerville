cmake_minimum_required(VERSION 3.16)
project(Baskerville)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE core_srcs
        "core/*.cpp"
        "core/*.h"
        "core/*.hpp"
        )

file(GLOB_RECURSE widget_srcs
        "app/*.h"
        "app/*.hpp"
        "app/*.cpp"
        )

file(GLOB_RECURSE dep_srcs
        "dep/*.cpp"
        "dep/*.h"
        "dep/*.hpp"
        )

add_executable(Baskerville main.cpp ${core_srcs} ${widget_srcs} ${dep_srcs})

target_compile_definitions(Baskerville PUBLIC MSDFGEN_USE_CPP11 BASKERVILLE_DEBUG_DRAW BASKERVILLE_IN_DEV)
target_link_libraries(Baskerville opengl32.lib comctl32.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL2/lib/SDL2.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL2/lib/SDL2main.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/nfd/lib/nfd.lib)
target_include_directories(Baskerville PUBLIC dep core app)