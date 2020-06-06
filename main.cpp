
#define NOMINMAX

#include "common.h"

#include <iostream>
#include <string>
#include <chrono>

#include "event.h"
#include "platform.h"
#include "font.h"
#include "root.h"
#include "shader.h"

#include "meta.h"

Platform::Manager platform;

Shader::Cache shader_cache;
Font::Cache font_cache;

Root* root;

using timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

int main() {

    if(platform.startup(1080)) {

        Font::Cache::load("editor", "LibreBaskerville-Regular.ttf", 33, 126, 24);

        root = new Root();

        //Explicitly call onWindowResize once for widgets to adjust to the windows initial size
        root->onWindowResize();

        std::chrono::duration<u64, std::nano> update_accumulator(0);
        std::chrono::duration<u64, std::milli> update_rate((u32)((1.0 / (f32)120) * 1000)); // Locked to 120, change later
        timestamp cur_time = std::chrono::high_resolution_clock::now();

        while (!platform.shouldQuit()) {
            timestamp new_time = std::chrono::high_resolution_clock::now();
            auto frame_time = std::chrono::duration<u64, std::nano>(new_time - cur_time);
            cur_time = new_time;
            update_accumulator += frame_time;
            u32 update_count = 0;
            while (update_accumulator >= update_rate) {

                platform.swap();
                platform.pollEvents();
                Event::Codepoint codepoint;
                Event::Macro macro;
                if(platform.windowResized()) {
                    root->onWindowResize();
                }
                while(platform.pollMacro(macro)) {
                    root->onMacro(macro);
                }
                while(platform.pollCodepoint(codepoint)) {
                    root->onCodepoint(codepoint);
                }
                root->update();
                update_accumulator -= update_rate;
                update_count++;
                if (update_count > 6) {
                    update_accumulator = std::chrono::duration<u64, std::nano>::zero();
                }
            }
            if (update_count > 0) {
                root->draw(platform.getDrawCallQueue());
                platform.executeDrawCalls();
            }
        }

        delete root;

        shader_cache.clear();
        font_cache.clear();
    }
    platform.shutdown();
    return 0;
}
