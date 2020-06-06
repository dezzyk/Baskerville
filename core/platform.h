//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"
#include "event.h"
#include "draw_call.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"

class Platform {
public:
    class Manager {
    public:
        struct State {
            Event::RingQueue<Event::Codepoint, 128> codepoint;
            Event::RingQueue<Event::Macro, 16> macro;
            std::optional<Event::WindowResize> window_resize;
        };
        b32 startup(u32 height);
        void shutdown();
        void pollEvents();
        b32 pollCodepoint(Event::Codepoint& value);
        b32 pollMacro(Event::Macro& value);
        b32 pollResize(Event::WindowResize& resize);
        void swap();
        b32 shouldQuit();
        void executeDrawCalls();
        Draw::CallQueue& getDrawCallQueue();
    };
    static glm::vec2 getViewportSize();
    static glm::vec2 getMousePos();
    static f32 getGlobalScaler();
private:
    static Draw::CallQueue call_queue;
    static Manager::State state;
    static GLFWwindow* window;
    static u32 target_height;
};