
#include "common.h"

#include <iostream>
#include <string>

#include "glad/src/glad.c"
#include "glfw/glfw3.h"

#include "event.h"
#include "window.h"
#include "widget.h"

Event::Manager event;
Window window;

int main() {
    window.startup(event);
    while (!window.shouldQuit()) {
        window.pollEvents();
    }
    window.shutdown();
    return 0;
}
