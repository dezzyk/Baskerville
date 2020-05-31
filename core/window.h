//
// Created by Feed on 5/26/2020.
//

#pragma once

#include "common.h"
#include "event.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"

class Window {
public:
    using Viewport = glm::vec2;
    struct State {
        Event::RingQueue<Event::Codepoint, 128> codepoint;
        Event::RingQueue<Event::Macro, 16> macro;
        std::optional<Event::WindowResize> window_resize;
        Viewport viewport;
    };
    b32 startup();
    void shutdown();
    void pollEvents();
    b32 pollCodepoint(Event::Codepoint& value);
    b32 pollMacro(Event::Macro& value);
    b32 pollResize(Event::WindowResize& resize);
    void swap();
    b32 shouldQuit();
    const Viewport& getViewport();
private:
    State m_state;
    GLFWwindow* m_window = nullptr;
};