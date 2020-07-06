//
// Created by Feed on 6/5/2020.
//

#include "platform.h"
#include "gl_err.h"

#include "glm/gtc/type_ptr.hpp"
#include "glad/src/glad.c"

#include <iostream>

std::vector<const Draw::Context*> Platform::draw_queue;
SDL_Window* Platform::window = nullptr;
SDL_GLContext  Platform::context;
f32 Platform::viewport_scaler = 1.0f;
u32 Platform::target_height = 0;
b32 Platform::should_close = false;
std::filesystem::path Platform::pref_path;

b32 Platform::Manager::startup(const char* app_name, const char* org_name, u32 height) {
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        pref_path = SDL_GetPrefPath(org_name, app_name);
        target_height = height;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        window = SDL_CreateWindow(app_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
}

void Platform::Manager::shutdown() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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
            } else if( e.key.keysym.sym == SDLK_e && SDL_GetModState() & KMOD_CTRL ) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Export;
            } else if( e.key.keysym.sym == SDLK_RETURN) {
                event.type = Meta::MakeType<Event::Macro>();
                event.value.macro = Event::Macro::Enter;
            }
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            // TODO mouse events
        } /*else if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            glm::vec2 size = getViewportSize();
            viewport_scaler = (size.y / target_height);
        }*/
        return true;
    }
    return false;
}

void Platform::Manager::swap() {
    SDL_GL_SwapWindow(window);
}

b32 Platform::Manager::shouldQuit() {
    return should_close;
}

std::vector<const Draw::Context*>& Platform::Manager::getDrawQueue() {
    return draw_queue;
}

void Platform::Manager::delay(u32 count) {
    SDL_Delay(count);
}

void Platform::Manager::executeDrawCalls() {
    glClear(GL_COLOR_BUFFER_BIT); CheckGLError();
    glm::vec2 viewport = getViewportSize();
    glm::mat4 proj = glm::ortho(0.0f, viewport.x, 0.0f, viewport.y);
    static glm::vec2 last_viewport = {0.0f, 0.0f};
    glm::vec2  cur_viewport = getViewportSize();
    if(viewport != last_viewport) {
        glViewport(0, 0, viewport.x, viewport.y);
        last_viewport = viewport;
    }
    for(auto draw : draw_queue) {
        if(draw->valid()) {
            if(draw->size() > 0) {
                glBindVertexArray(draw->getVAO().value());
                glUseProgram(draw->getShader()->getHandle().value());
                glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(proj));
                glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(draw->model));
                for (int i = 0; i < draw->uniforms.size(); ++i) {
                    if (draw->uniforms[i].type != Meta::MakeType<Meta::Empty>()) {
                        Meta::Type type = draw->uniforms[i].type;
                        if (type == Meta::MakeType<glm::mat4>()) {
                            glUniformMatrix4fv(i + 2, 1, GL_FALSE, glm::value_ptr(draw->uniforms[i].value.mat4));
                        } else if (type == Meta::MakeType<glm::vec4>()) {
                            glUniform4f(i + 2, draw->uniforms[i].value.vec4.x, draw->uniforms[i].value.vec4.y,
                                        draw->uniforms[i].value.vec4.z, draw->uniforms[i].value.vec4.w);
                        } else if (type == Meta::MakeType<glm::vec3>()) {
                            glUniform3f(i + 2, draw->uniforms[i].value.vec3.x, draw->uniforms[i].value.vec3.y,
                                        draw->uniforms[i].value.vec3.z);
                        } else if (type == Meta::MakeType<glm::vec2>()) {
                            glUniform2f(i + 2, draw->uniforms[i].value.vec2.x, draw->uniforms[i].value.vec2.y);
                        } else if (type == Meta::MakeType<f32>()) {
                            glUniform1f(i + 2, draw->uniforms[i].value.u_f32);
                        } else if (type == Meta::MakeType<u32>()) {
                            glUniform1ui(i + 2, draw->uniforms[i].value.u_u32);
                        } else if (type == Meta::MakeType<i32>()) {
                            glUniform1i(i + 2, draw->uniforms[i].value.u_i32);
                        } else if (type == Meta::MakeType<Draw::Uniform::Texture>()) {
                            glUniform1i(i + 2, draw->uniforms[i].value.texture.index);
                            glActiveTexture(GL_TEXTURE0 + draw->uniforms[i].value.texture.index);
                            glBindTexture(GL_TEXTURE_2D, draw->uniforms[i].value.texture.handle);
                        } else if (type == Meta::MakeType<Draw::Uniform::ArrayTexture>()) {
                            glUniform1i(i + 2, draw->uniforms[i].value.texture.index);
                            glActiveTexture(GL_TEXTURE0 + draw->uniforms[i].value.array_texture.index);
                            glBindTexture(GL_TEXTURE_2D_ARRAY, draw->uniforms[i].value.array_texture.handle);
                        }
                    }
                }
                for (int i = 0; i < draw->buffers.size(); ++i) {
                    if (draw->buffers[i].handle != 0) {
                        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, draw->buffers[i].handle);
                    }
                }
                Draw::Quad size_quad; // I hate this
                glDrawArrays(GL_TRIANGLES, 0, draw->size() * size_quad.vertices.size());
                CheckGLError();
            }
        }
    }
    glBindVertexArray(0);
    draw_queue.resize(0);
}

const glm::vec2 Platform::getViewportSize() {
    int x, y;
    SDL_GL_GetDrawableSize(window, &x, &y);
    return {x, y};
}

const glm::vec2 Platform::getMousePos() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

const std::filesystem::path& Platform::getPrefPath() {
    return pref_path;
}

const f32 Platform::getViewportScaler() {
    glm::vec2 size = getViewportSize();
    return (size.y / target_height);
}

const std::string& Platform::getDatPath() {
#ifdef BASKERVILLE_IN_DEV
    static const std::string data_path = "../data/";
#else
    static const std::string data_path = "data/";
#endif
    return data_path;
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