
#define NOMINMAX

#include "common.h"
#include "event.h"
#include "platform.h"
#include "font.h"
#include "root.h"
#include "shader.h"
#include "project.h"
#include "meta.h"
#include "cache.h"

#include <iostream>
#include <string>
#include <chrono>

Platform::Manager platform;
Shader::Cache shader_cache;
Font::Cache font_cache;
CacheBank cache;

Root* root;

using timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

int main(int argc, char *argv[]) {

    if(platform.startup("Baskerville", "Saltpowered", 1080)) {

        Font::Cache::load("editor", "LibreBaskerville-Regular.ttf", 33, 126, 24);

        // Preload shaders
        std::string vert =
#include "shader/msdf_draw.vert"
        ;
        std::string frag =
#include "shader/msdf_draw.frag"
        ;
        Shader::Cache::load("msdf_draw", vert, frag);

        root = new Root(cache);

        timestamp cur_time = std::chrono::high_resolution_clock::now();

        b32 first_frame = true;
        glm::vec2  last_viewport = Platform::getViewportSize();

        while (!platform.shouldQuit()) {
            timestamp new_time = std::chrono::high_resolution_clock::now();
            f64 frame_time = std::chrono::duration<f64, std::milli>(new_time - cur_time).count() / 1000;
            cur_time = new_time;

            u32 base_delay = (1000/90);
            u32 frame_time_rounded = (u32)(frame_time * 1000.0);
            u32 total_delay = 0;
            if(frame_time_rounded > base_delay) {
                // If frame time is over double delay time, then just dont delay. Can happen when moving/resizing
                // the window or performing anything else that blocks.
                if(frame_time_rounded <= base_delay * 2) {
                    total_delay = base_delay - (frame_time_rounded - base_delay);
                }
            } else {
                total_delay = base_delay;
            }
            platform.delay(total_delay);

            Draw::RedrawFlag redraw;
            if (first_frame) {
                redraw = true;
                first_frame = false;
            }

            redraw = root->update(frame_time);

            Event::Container event;
            while (platform.pollEvents(event)) {
                if (event.type == Meta::MakeType<Event::TextInput>()) {
                    redraw = root->onTextInput(event.value.text);
                } else if (event.type == Meta::MakeType<Event::Macro>()) {
                    redraw = root->onMacro(event.value.macro);
                } else if (event.type == Meta::MakeType<Event::MouseClick>()) {
                    redraw = root->onMouseClick(event.value.mouseClick);
                }
            }

            glm::vec2 cur_viewport = Platform::getViewportSize();
            if (cur_viewport != last_viewport) {
                redraw = true;
                last_viewport = cur_viewport;
            }

            if(redraw) {
                root->draw(platform.getDrawCallQueue(), platform.getViewportScaler());
                platform.executeDrawCalls();
                platform.swap();
            }

        }

        delete root;

        shader_cache.clear();
        font_cache.clear();
    }
    platform.shutdown();
    return 0;
}
