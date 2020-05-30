
#define NOMINMAX

#include "common.h"

#include <iostream>
#include <string>
#include <chrono>

#include "event.h"
#include "window.h"
#include "font.h"
#include "baskerville.h"

Event::Manager events;
Window window;
Baskerville baskerville;

using timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

int main() {

    if(window.startup(events)) {
        Font::Cache::load("editor_text", "LibreBaskerville-Regular.ttf", 32, 48, 32);

        std::chrono::duration<u64, std::nano> update_accumulator(0);
        std::chrono::duration<u64, std::milli> update_rate((u32)((1.0 / (f32)120) * 1000)); // Locked to 120, change later
        timestamp cur_time = std::chrono::high_resolution_clock::now();

        while (!window.shouldQuit()) {
            timestamp new_time = std::chrono::high_resolution_clock::now();
            auto frame_time = std::chrono::duration<u64, std::nano>(new_time - cur_time);
            cur_time = new_time;
            update_accumulator += frame_time;
            u32 update_count = 0;
            while (update_accumulator >= update_rate) {

                window.pollEvents();
                Event::Codepoint codepoint;
                Event::Resize resize;
                Event::Macro macro;
                while(events.pollResize(resize)) {
                    baskerville.handleResize(resize);
                }
                while(events.pollMacro(macro)) {
                    baskerville.handleMacro(macro);
                }
                while(events.pollCodepoint(codepoint)) {
                    baskerville.handleCodepoint(codepoint);
                }

                update_accumulator -= update_rate;
                update_count++;
                if (update_count > 3) {
                    update_accumulator = std::chrono::duration<u64, std::nano>::zero();
                }
            }
        }

        Font::Cache::clear();
    }
    window.shutdown();
    return 0;
}
