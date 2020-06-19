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
        b32 startup(u32 height);
        void shutdown();
        void pollEvents();
        b32 pollCodepoint(Event::Codepoint& value);
        b32 pollMacro(Event::Macro& value);
        b32 pollMouseClick(Event::MouseClick& value);
        void swap();
        b32 shouldQuit();
        void executeDrawCalls();
        Draw::CallQueue& getDrawCallQueue();
        f32 getViewportScaler();
    };
    static const glm::vec2 getViewportSize();
    static const glm::vec2 getMousePos();
private:
    static Draw::CallQueue call_queue;
    static GLFWwindow* window;
    static Event::RingQueue<Event::Codepoint, 128> codepoint;
    static Event::RingQueue<Event::Macro, 16> macro;
    static Event::RingQueue<Event::MouseClick, 16> mouse_click;
    static f32 viewport_scaler;
    static u32 target_height;
};