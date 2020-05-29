
#define NOMINMAX

#include "common.h"

#include <iostream>
#include <string>

#include "glad/src/glad.c"
#include "glfw/glfw3.h"

#include "event.h"
#include "window.h"
#include "widget.h"
#include "font.h"

Event::Manager event;
Window window;

Font font;

int main() {
    std::cout << (int)'A' << std::endl;
    window.startup(event);
    font = Font("LibreBaskerville-Regular.ttf", 96, 96, 64);
    while (!window.shouldQuit()) {
        window.pollEvents();
    }
    window.shutdown();
    return 0;
}
