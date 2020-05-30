//
// Created by Feed on 5/26/2020.
//

//#include "glad/src/glad.c"
#include "window.h"
#include "glad/src/glad.c"

#include <iostream>

b32 Window::startup(Event::Manager& event) {
    m_event_manager = &event;
    if(glfwInit()) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_window = glfwCreateWindow(960, 540, "test", nullptr, nullptr);
        if (!m_window) {
            std::cout << "Failed to create window" << std::endl;
            return false;
        }
        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, &m_event_manager->m_queue_state);
        glfwSetCharCallback(m_window, [](GLFWwindow *window, u32 value) -> void {
            Event::QueueState *queue = reinterpret_cast<Event::QueueState *>(glfwGetWindowUserPointer(window));
            queue->codepoint.push({value});
        });
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            Event::QueueState* queue = reinterpret_cast<Event::QueueState* >(glfwGetWindowUserPointer(window));
            if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
                queue->macro.push({ Event::Macro::Type::Backspace });
            } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                queue->macro.push({ Event::Macro::Type::Escape });
            } else if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
                queue->macro.push({ Event::Macro::Type::Tab });
            } else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
                queue->macro.push({ Event::Macro::Type::Enter });
            } else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
                queue->macro.push({ Event::Macro::Type::Delete });
            } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT ||
                       glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_REPEAT) {
                if (key == GLFW_KEY_A) {
                    queue->macro.push({Event::Macro::Type::SelectAll} );
                } else if (key == GLFW_KEY_C) {
                    queue->macro.push({Event::Macro::Type::Copy} );
                } else if (key == GLFW_KEY_V) {
                    queue->macro.push({Event::Macro::Type::Paste} );
                } else if (key == GLFW_KEY_X) {
                    queue->macro.push({Event::Macro::Type::Cut} );
                } else if (key == GLFW_KEY_S) {
                    queue->macro.push({Event::Macro::Type::Save} );
                }
            }
        });
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int w, int h) {
            Event::QueueState *queue = reinterpret_cast<Event::QueueState *>(glfwGetWindowUserPointer(window));
            queue->resize.push({(u32) w, (u32) h});
        });
        int res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0) {
            std::cout << "Failed to load glad" << std::endl;
            return false;
        };
        return true;
    }
    return false;
}

void Window::shutdown() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::pollEvents() {
    glfwPollEvents();
}

b32 Window::shouldQuit() {
    return glfwWindowShouldClose(m_window);
}
