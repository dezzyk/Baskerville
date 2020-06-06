//
// Created by Feed on 6/5/2020.
//

#include "platform.h"
#include "gl_err.h"

#include "glm/gtc/type_ptr.hpp"
#include "glad/src/glad.c"

#include <iostream>

Draw::CallQueue Platform::call_queue;
Platform::Manager::State Platform::state;
GLFWwindow* Platform::window = nullptr;
u32 Platform::target_height = 0;

b32 Platform::Manager::startup(u32 height) {
    if(glfwInit()) {
        target_height = height;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(960, 540, "test", nullptr, nullptr);
        if (!window) {
            std::cout << "Failed to create window" << std::endl;
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, &state);
        glfwSetCharCallback(window, [](GLFWwindow *window, u32 value) -> void {
            Platform::Manager::State* state = reinterpret_cast<Platform::Manager::State* >(glfwGetWindowUserPointer(window));
            state->codepoint.push({value});
        });
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            Platform::Manager::State* state = reinterpret_cast<Platform::Manager::State* >(glfwGetWindowUserPointer(window));
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
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int w, int h) {
            Platform::Manager::State* state = reinterpret_cast<Platform::Manager::State* >(glfwGetWindowUserPointer(window));
            glViewport(0, 0, w, h);
            state->window_resize = {(u32) w, (u32) h };
        });
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to load glad" << std::endl;
            return false;
        };
        state.window_resize = getViewportSize();
        glViewport(0, 0, state.window_resize->x, state.window_resize->y);
        glEnable(GL_BLEND); CheckGLError();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CheckGLError();
        glClearColor(0.0, 0.0, 0.0, 1.0); CheckGLError();
        // Go ahead and set a window_resize event so that widgets adjust to the window size
        return true;
    }
    return false;
}

void Platform::Manager::shutdown() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Platform::Manager::pollEvents() {
    glfwPollEvents();
}

b32 Platform::Manager::pollCodepoint(Event::Codepoint& value) {
    if(state.codepoint.size() > 0) {
        value = state.codepoint.front();
        state.codepoint.pop();
        return true;
    }
    return false;
}

b32 Platform::Manager::pollMacro(Event::Macro& value) {
    if(state.macro.size() > 0) {
        value = state.macro.front();
        state.macro.pop();
        return true;
    }
    return false;
}

b32 Platform::Manager::pollResize(Event::WindowResize& resize) {
    if(state.window_resize.has_value()) {
        resize = state.window_resize.value();
        state.window_resize = std::nullopt;
        return true;
    }
    return false;
}

void Platform::Manager::swap() {
    glfwSwapBuffers(window);
}

b32 Platform::Manager::shouldQuit() {
    return glfwWindowShouldClose(window);
}

Draw::CallQueue& Platform::Manager::getDrawCallQueue() {
    return call_queue;
}

void Platform::Manager::executeDrawCalls() {
    glClear(GL_COLOR_BUFFER_BIT); CheckGLError();
    glm::vec2 viewport = getViewportSize();
    glm::mat4 proj = glm::ortho(0.0f, viewport.x, 0.0f, viewport.y);
    for(auto& draw : call_queue) {
        if(draw.shader != nullptr) {
            if(draw.shader->getHandle().has_value()) {
                if(draw.context->valid()) {
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
                    glBindVertexArray(draw.context->getVAO().value());
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
}

glm::vec2 Platform::getViewportSize() {
    int x, y;
    glfwGetFramebufferSize(window, &x, & y);
    return {x, y};
}

glm::vec2 Platform::getMousePos() {
    f64 x, y;
    glfwGetCursorPos(window, &x, & y);
    return {x, y};
}

f32 Platform::getGlobalScaler() {
    glm::vec2 cur_viewport = getViewportSize();
    return (cur_viewport.y / target_height);
}