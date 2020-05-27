cmake_minimum_required(VERSION 3.16)
project(saltnovel)

set(CMAKE_CXX_STANDARD 17)

add_executable(saltnovel main.cpp core/common.h dep/glad/glad.h core/widget.h core/event.h core/src/event.cpp core/window.h core/src/window.cpp core/sprite.h core/renderer.h core/draw.h)

target_link_libraries(saltnovel ${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw/lib/glfw3.lib)
target_include_directories(saltnovel PUBLIC dep core)