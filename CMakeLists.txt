cmake_minimum_required(VERSION 3.16)
project(saltnovel)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE core_srcs
        "core/*.cpp"
        "core/*.h"
        "core/*.hpp"
        )

file(GLOB_RECURSE widget_srcs
        "widget/*.h"
        "widget/*.hpp"
        "widget/*.cpp"
        )

file(GLOB_RECURSE dep_srcs
        "dep/*.cpp"
        "dep/*.h"
        "dep/*.hpp"
        )

add_executable(saltnovel main.cpp ${core_srcs} ${widget_srcs} ${dep_srcs})

target_compile_definitions(saltnovel PUBLIC MSDFGEN_USE_CPP11)
target_link_libraries(saltnovel ${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw/lib/glfw3.lib)
target_include_directories(saltnovel PUBLIC dep core)