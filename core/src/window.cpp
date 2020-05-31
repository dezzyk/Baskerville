//
// Created by Feed on 5/26/2020.
//

//#include "glad/src/glad.c"
#include "window.h"
#include "glad/src/glad.c"
#include "gl_err.h"

#include <iostream>

b32 Window::startup() {
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
        glfwSetWindowUserPointer(m_window, &m_state);
        glfwSetCharCallback(m_window, [](GLFWwindow *window, u32 value) -> void {
            Window::State* state = reinterpret_cast<Window::State* >(glfwGetWindowUserPointer(window));
            state->codepoint.push({value});
        });
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            Window::State* state = reinterpret_cast<Window::State* >(glfwGetWindowUserPointer(window));
            if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::Backspace });
            } else if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::Escape });
            } else if (key == GLFW_KEY_TAB && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::Tab });
            } else if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::Enter });
            } else if (key == GLFW_KEY_DELETE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::Delete });
            } else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::ArrowUp });
            } else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::ArrowDown });
            } else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::ArrowLeft });
            } else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                state->macro.push({Event::Macro::ArrowRight });
            } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT ||
                       glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_REPEAT) {
                if (key == GLFW_KEY_A) {
                    state->macro.push({Event::Macro::SelectAll} );
                } else if (key == GLFW_KEY_C) {
                    state->macro.push({Event::Macro::Copy} );
                } else if (key == GLFW_KEY_V) {
                    state->macro.push({Event::Macro::Paste} );
                } else if (key == GLFW_KEY_X) {
                    state->macro.push({Event::Macro::Cut} );
                } else if (key == GLFW_KEY_S) {
                    state->macro.push({Event::Macro::Save} );
                }
            }
        });
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int w, int h) {
            Window::State* state = reinterpret_cast<Window::State* >(glfwGetWindowUserPointer(window));
            state->viewport = { (u32) w, (u32) h };
            glViewport(0, 0, w, h);
            state->window_resize = {(u32) w, (u32) h };
        });
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to load glad" << std::endl;
            return false;
        };
        glViewport(0, 0, 960, 540);
        glEnable(GL_BLEND); CheckGLError();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CheckGLError();
        glClearColor(0.0, 0.0, 0.0, 1.0); CheckGLError();
        // Go ahead and set a window_resize event so that widgets adjust to the window size
        m_state.window_resize = {(u32) 960, (u32) 540 };
        m_state.viewport = { (u32) 960, (u32) 540 };
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

b32 Window::pollCodepoint(Event::Codepoint& value) {
    if(m_state.codepoint.size() > 0) {
        value = m_state.codepoint.front();
        m_state.codepoint.pop();
        return true;
    }
    return false;
}

b32 Window::pollMacro(Event::Macro& value) {
    if(m_state.macro.size() > 0) {
        value = m_state.macro.front();
        m_state.macro.pop();
        return true;
    }
    return false;
}

b32 Window::pollResize(Event::WindowResize& resize) {
    if(m_state.window_resize.has_value()) {
        resize = m_state.window_resize.value();
        m_state.window_resize = std::nullopt;
        return true;
    }
    return false;
}

void Window::swap() {
    glfwSwapBuffers(m_window);
}

b32 Window::shouldQuit() {
    return glfwWindowShouldClose(m_window);
}

const Window::Viewport& Window::getViewport() {
    return m_state.viewport;
}