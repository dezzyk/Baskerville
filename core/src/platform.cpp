//
// Created by Feed on 6/5/2020.
//

#include "platform.h"
#include "gl_err.h"

#include "glm/gtc/type_ptr.hpp"
#include "glad/src/glad.c"

#include <iostream>

Draw::CallQueue Platform::call_queue;
//Event::RingQueue<Event::Codepoint, 128> Platform::codepoint;
//Event::RingQueue<Event::Macro, 16> Platform::macro;
//Event::RingQueue<Event::MouseClick, 16> Platform::mouse_click;
SDL_Window* Platform::window = nullptr;
SDL_GLContext  Platform::context;
f32 Platform::viewport_scaler = 1.0f;
u32 Platform::target_height = 0;
b32 Platform::should_close = false;

b32 Platform::Manager::startup(u32 height) {
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        target_height = height;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        window = SDL_CreateWindow("Baskerville", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  960, 540, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI);
        context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, context);
        SDL_GL_SetSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cout << "Failed to load glad" << std::endl;
            return false;
        };

        glm::vec2 viewport = getViewportSize();
        glViewport(0, 0, viewport.x, viewport.y);
        glEnable(GL_BLEND); CheckGLError();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CheckGLError();
        glClearColor(0.0, 0.0, 0.0, 1.0); CheckGLError();

        SDL_StartTextInput();
        return true;
    }
    return false;
    /*if(glfwInit()) {
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
        glfwSwapInterval(1);
        glfwSetCharCallback(window, [](GLFWwindow *window, u32 value) -> void {
            codepoint.push({value});
        });
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::Backspace });
            } else if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::Escape });
            } else if (key == GLFW_KEY_TAB && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::Tab });
            } else if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::Enter });
            } else if (key == GLFW_KEY_DELETE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::Delete });
            } else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::ArrowUp });
            } else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::ArrowDown });
            } else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::ArrowLeft });
            } else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
                macro.push({Event::Macro::ArrowRight });
            } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT ||
                       glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_REPEAT) {
                if (key == GLFW_KEY_A) {
                    macro.push({Event::Macro::SelectAll} );
                } else if (key == GLFW_KEY_C) {
                    macro.push({Event::Macro::Copy} );
                } else if (key == GLFW_KEY_V) {
                    macro.push({Event::Macro::Paste} );
                } else if (key == GLFW_KEY_X) {
                    macro.push({Event::Macro::Cut} );
                } else if (key == GLFW_KEY_S) {
                    macro.push({Event::Macro::Save} );
                    std::cout << "saved" << std::endl;
                } else if (key == GLFW_KEY_O) {
                    macro.push({Event::Macro::Open} );
                } else if (key == GLFW_KEY_N) {
                    macro.push({Event::Macro::New} );
                }
            }
        });
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int w, int h) {
            glViewport(0, 0, w, h);
            viewport_scaler = ((f32)h / target_height);
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
            if(action == GLFW_PRESS && (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT)) {
                // GL drops with y pos 0 at bottom, GLFW reports with y pos 0 at top, reverse the y pos.
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                glm::vec2 viewport = getViewportSize();
                b32 button = true;
                if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                    button = false;
                }
                mouse_click.push({{x, viewport.y - y}, button});
            }
        });
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to load glad" << std::endl;
            return false;
        };
        glm::vec2 viewport = getViewportSize();
        glViewport(0, 0, viewport.x, viewport.y);
        glEnable(GL_BLEND); CheckGLError();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CheckGLError();
        glClearColor(0.0, 0.0, 0.0, 1.0); CheckGLError();
        return true;
    }
    return false;*/
}

void Platform::Manager::shutdown() {
    //glfwDestroyWindow(window);
    //glfwTerminate();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/*void Platform::Manager::pollEvents() {
    glfwPollEvents();
}*/

b32 Platform::Manager::pollEvents(Event::Container& event) {
    SDL_Event e;
    event.type = {};
    if(SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            should_close = true;
            return false;
        } else if (e.type == SDL_TEXTINPUT) {
            if(!(SDL_GetModState() & KMOD_CTRL &&
            ( e.text.text[ 0 ] == 'c' ||
            e.text.text[ 0 ] == 'C' ||
            e.text.text[ 0 ] == 'v' ||
            e.text.text[ 0 ] == 'V' ||
            e.text.text[ 0 ] == 'n' ||
            e.text.text[ 0 ] == 'N' ||
            e.text.text[ 0 ] == 'o' ||
            e.text.text[ 0 ] == 'O' ||
            e.text.text[ 0 ] == 's' ||
            e.text.text[ 0 ] == 'S' ||
            e.text.text[ 0 ] == 'x' ||
            e.text.text[ 0 ] == 'X' ))) {
                std::string_view str(e.text.text);
                event.type = Meta::MakeType<Event::TextInput>();
                strcpy(event.value.text.value, e.text.text);
                //str.copy(event.value.text.value, 0, str.size());
                return true;
            }
        } else if( e.type == SDL_KEYDOWN ) {
            if( e.key.keysym.sym == SDLK_BACKSPACE) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Backspace;
            } else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL ) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Copy;
            } else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL ) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Paste;
            } else if( e.key.keysym.sym == SDLK_s && SDL_GetModState() & KMOD_CTRL ) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Save;
            } else if( e.key.keysym.sym == SDLK_o && SDL_GetModState() & KMOD_CTRL ) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Open;
            } else if( e.key.keysym.sym == SDLK_n && SDL_GetModState() & KMOD_CTRL ) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::New;
            } else if( e.key.keysym.sym == SDLK_KP_ENTER && SDL_GetModState() & KMOD_CTRL ) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Enter;
            }
            return true;
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            // TODO mouse events
            return true;
        } else if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            glm::vec2 size = getViewportSize();
            glViewport(0, 0, size.x, size.y);
            viewport_scaler = (size.y / target_height);
            return true;
        }
    }
}

/*b32 Platform::Manager::pollCodepoint(Event::Codepoint& value) {
    if(codepoint.size() > 0) {
        value = codepoint.front();
        codepoint.pop();
        return true;
    }
    return false;
}

b32 Platform::Manager::pollMacro(Event::Macro& value) {
    if(macro.size() > 0) {
        value = macro.front();
        macro.pop();
        return true;
    }
    return false;
}

b32 Platform::Manager::pollMouseClick(Event::MouseClick& value) {
    if(mouse_click.size() > 0) {
        value = mouse_click.front();
        mouse_click.pop();
        return true;
    }
    return false;
}*/

void Platform::Manager::swap() {
    //glfwSwapBuffers(window);
    SDL_GL_SwapWindow(window);
}

b32 Platform::Manager::shouldQuit() {
    //return glfwWindowShouldClose(window);
    return should_close;
}

Draw::CallQueue& Platform::Manager::getDrawCallQueue() {
    return call_queue;
}

f32 Platform::Manager::getViewportScaler() {
    return viewport_scaler;
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
                    Draw::Box size_box;
                    glDrawArrays(GL_TRIANGLES, 0, draw.count * size_box.vertices.size());
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
    call_queue.resize(0);
}

const glm::vec2 Platform::getViewportSize() {
    int x, y;
    //glfwGetFramebufferSize(window, &x, & y);
    SDL_GL_GetDrawableSize(window, &x, &y);
    return {x, y};
}

const glm::vec2 Platform::getMousePos() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    //glfwGetCursorPos(window, &x, & y);
    return {x, y};
}

void Platform::errorMessageBox(const char* title, const char* msg) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             title,
                             msg,
                             NULL);
}

void Platform::cautionMessageBox(const char* title, const char* msg) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
                             title,
                             msg,
                             NULL);
}

void Platform::infoMessageBox(const char* title, const char* msg) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             title,
                             msg,
                             NULL);
}

const b32 Platform::errorOptionBox(const char* title, const char* msg, b32& res) {
    const SDL_MessageBoxButtonData buttons[] = {
            { 0, 0, "no" },
            { 0, 1, "yes" }
    };
    const SDL_MessageBoxColorScheme colorScheme = {
            { /* .colors (.r, .g, .b) */
                    /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                    { 255,   0,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                    {   0, 255,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                    { 255, 255,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                    {   0,   0, 255 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                    { 255,   0, 255 }
            }
    };
    const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_ERROR, /* .flags */
            NULL, /* .window */
            title, /* .title */
            msg, /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return false;
    }
    res = buttonid;
    return true;

}

const b32 Platform::cautionOptionBox(const char* title, const char* msg, b32& res) {
    const SDL_MessageBoxButtonData buttons[] = {
            { 0, 0, "no" },
            { 0, 1, "yes" }
    };
    const SDL_MessageBoxColorScheme colorScheme = {
            { /* .colors (.r, .g, .b) */
                    /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                    { 255,   0,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                    {   0, 255,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                    { 255, 255,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                    {   0,   0, 255 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                    { 255,   0, 255 }
            }
    };
    const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_WARNING, /* .flags */
            NULL, /* .window */
            title, /* .title */
            msg, /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return false;
    }
    res = buttonid;
    return true;
}

const b32 Platform::infoOptionBox(const char* title, const char* msg, b32& res) {
    const SDL_MessageBoxButtonData buttons[] = {
            { 0, 0, "no" },
            { 0, 1, "yes" }
    };
    const SDL_MessageBoxColorScheme colorScheme = {
            { /* .colors (.r, .g, .b) */
                    /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                    { 255,   0,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                    {   0, 255,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                    { 255, 255,   0 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                    {   0,   0, 255 },
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                    { 255,   0, 255 }
            }
    };
    const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            NULL, /* .window */
            title, /* .title */
            msg, /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return false;
    }
    res = buttonid;
    return true;
}