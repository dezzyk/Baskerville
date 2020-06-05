//
// Created by Feed on 5/26/2020.
//

//#include "glad/src/glad.c"
#include "window.h"
#include "gl_err.h"

#include "glm/gtc/type_ptr.hpp"
#include "glad/src/glad.c"

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

Draw::CallQueue& Window::getDrawBuffer() {
    return m_draw_buffer;
}

void Window::draw() {
    glClear(GL_COLOR_BUFFER_BIT); CheckGLError();
    glm::mat4 proj = glm::ortho(0.0f, m_state.viewport.x, 0.0f, m_state.viewport.y);
    for(auto& draw : m_draw_buffer) {
        if(draw.shader != nullptr) {
            if(draw.shader->getHandle().has_value()) {
                if(draw.handles.vao.has_value()) {
                    glUseProgram(draw.shader->getHandle().value());
                    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(proj));
                    for (int i = 0; i < draw.uniforms.size(); ++i) {
                        if (draw.uniforms[i].type != Meta::MakeType<Meta::Empty>()) {
                            Meta::Type type = draw.uniforms[i].type;
                            if (type == Meta::MakeType<glm::mat4>()) {
                                glUniformMatrix4fv(i + 1, 1, GL_FALSE, glm::value_ptr(draw.uniforms[i].value.mat4));
                            } else if (type == Meta::MakeType<glm::vec4>()) {
                                glUniform4f(i + 1, draw.uniforms[i].value.vec4.x, draw.uniforms[i].value.vec4.y,
                                            draw.uniforms[i].value.vec4.z, draw.uniforms[i].value.vec4.w);
                            } else if (type == Meta::MakeType<glm::vec3>()) {
                                glUniform3f(i + 1, draw.uniforms[i].value.vec3.x, draw.uniforms[i].value.vec3.y,
                                            draw.uniforms[i].value.vec3.z);
                            } else if (type == Meta::MakeType<glm::vec2>()) {
                                glUniform2f(i + 1, draw.uniforms[i].value.vec2.x, draw.uniforms[i].value.vec2.y);
                            } else if (type == Meta::MakeType<f32>()) {
                                glUniform1f(i + 1, draw.uniforms[i].value.u_f32);
                            } else if (type == Meta::MakeType<u32>()) {
                                glUniform1ui(i + 1, draw.uniforms[i].value.u_u32);
                            } else if (type == Meta::MakeType<i32>()) {
                                glUniform1i(i + 1, draw.uniforms[i].value.u_i32);
                            } else if (type == Meta::MakeType<Draw::Uniform::Texture>()) {
                                glUniform1i(i + 1, draw.uniforms[i].value.texture.index);
                                glActiveTexture(GL_TEXTURE0 + draw.uniforms[i].value.texture.index);
                                glBindTexture(GL_TEXTURE_2D, draw.uniforms[i].value.texture.handle);
                            } else if (type == Meta::MakeType<Draw::Uniform::ArrayTexture>()) {
                                glUniform1i(i + 1, draw.uniforms[i].value.texture.index);
                                glActiveTexture(GL_TEXTURE0 + draw.uniforms[i].value.array_texture.index);
                                glBindTexture(GL_TEXTURE_2D_ARRAY, draw.uniforms[i].value.array_texture.handle);
                            }
                        }
                    }
                    for (int i = 0; i < draw.buffers.size(); ++i) {
                        if (draw.buffers[i].handle != 0) {
                            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, draw.buffers[i].handle);
                        }
                    }
                    glBindVertexArray(draw.handles.vao.value());
                    CheckGLError();
                    glDrawArrays(GL_TRIANGLES, 0, draw.count);
                    CheckGLError();
                    glBindVertexArray(0);
                } else {
                    std::cout << "Call handles is equal to nullopt, skipping." << std::endl;
                }
            } else {
                std::cout << "Call shader valid but handle is equal to nullopt, skipping." << std::endl;
            }
        } else {
            std::cout << "Call shader equal to nullptr, skipping." << std::endl;
        }
    }
    m_draw_buffer.resize(0);
}