
#define NOMINMAX

#include "common.h"

#include <iostream>
#include <string>
#include <chrono>

#include "event.h"
#include "window.h"
#include "font.h"
#include "root.h"
#include "shader.h"

#include "meta.h"

Window window;

Shader::Cache shader_cache;
Font::Cache font_cache;

Root* root;

using timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

int main() {

    if(window.startup()) {

        Font::Cache::load("editor", "LibreBaskerville-Regular.ttf", 33, 126, 96);

        root = new Root();

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

                window.swap();
                window.pollEvents();
                Event::Codepoint codepoint;
                Event::WindowResize resize;
                Event::Macro macro;
                while(window.pollResize(resize)) {
                    root->onWindowResize(resize);
                }
                while(window.pollMacro(macro)) {
                    root->onMacro(macro);
                }
                while(window.pollCodepoint(codepoint)) {
                    root->onCodepoint(codepoint);
                }

                update_accumulator -= update_rate;
                update_count++;
                if (update_count > 6) {
                    update_accumulator = std::chrono::duration<u64, std::nano>::zero();
                }
            }
            if (update_count > 0) {
                root->draw(window.getDrawBuffer());
                window.draw();
            }
        }

        delete root;

        shader_cache.clear();
        font_cache.clear();
    }
    window.shutdown();
    return 0;
}
