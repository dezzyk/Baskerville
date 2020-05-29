
#define NOMINMAX

#include "common.h"

#include <iostream>
#include <string>

#include "event.h"
#include "window.h"
#include "widget.h"
#include "font.h"

Event::Manager event;
Window window;

Font font;

int main() {
    std::cout << (int)'A' << std::endl;
    if(window.startup(event)) {
        font = Font("LibreBaskerville-Regular.ttf", 32, 48, 32);
        while (!window.shouldQuit()) {
            window.pollEvents();
        }
    }
    window.shutdown();
    return 0;
}
