//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"
#include "event.h"
#include "draw_call.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "SDL2/SDL.h"

class Platform {
public:
    class Manager {
    public:
        b32 startup(u32 height);
        void shutdown();
        b32 pollEvents(Event::Container& event);
        void swap();
        b32 shouldQuit();
        void executeDrawCalls();
        Draw::CallQueue& getDrawCallQueue();
        f32 getViewportScaler();
    };
    static const glm::vec2 getViewportSize();
    static const glm::vec2 getMousePos();
    static void errorMessageBox(const char* title, const char* msg);
    static void cautionMessageBox(const char* title, const char* msg);
    static void infoMessageBox(const char* title, const char* msg);
    static const b32 errorOptionBox(const char* title, const char* msg, b32& res);
    static const b32 cautionOptionBox(const char* title, const char* msg, b32& res);
    static const b32 infoOptionBox(const char* title, const char* msg, b32& res);
private:
    static Draw::CallQueue call_queue;
    static SDL_Window* window;
    static SDL_GLContext context;
    static f32 viewport_scaler;
    static u32 target_height;
    static b32 should_close;
};