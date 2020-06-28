
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

        while (!platform.shouldQuit()) {
            timestamp new_time = std::chrono::high_resolution_clock::now();
            f32 frame_time = std::chrono::duration<f32, std::milli>(new_time - cur_time).count() / 1000;
            cur_time = new_time;

                Event::Container event;
                while(platform.pollEvents(event)) {
                    if(event.type == Meta::MakeType<Event::TextInput>()) {
                        root->onTextInput(event.value.text);
                    } else if(event.type == Meta::MakeType<Event::Macro>()) {
                        root->onMacro(event.value.macro);
                    } else if(event.type == Meta::MakeType<Event::MouseClick>()) {
                        root->onMouseClick(event.value.mouseClick);
                    }
                }

                root->update(frame_time);

                root->draw(platform.getDrawCallQueue(), platform.getViewportScaler());
                platform.executeDrawCalls();
                platform.swap();
        }

        delete root;

        shader_cache.clear();
        font_cache.clear();
    }
    platform.shutdown();
    return 0;
}
