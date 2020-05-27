//
// Created by Feed on 5/26/2020.
//

#define pragma once

#include "common.h"
#include "event.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

class Window {
public:
    void startup(Event::Manager& event);
    void shutdown();
    void pollEvents();
    b32 shouldQuit();
private:
    Event::Manager* m_event_manager = nullptr;
    GLFWwindow* m_window = nullptr;
};