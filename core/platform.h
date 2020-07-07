//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"
#include "event.h"
#include "draw.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "SDL2/SDL.h"

#include <filesystem>

class Platform {
public:
    class Manager {
    public:
        b32 startup(const char* app_name, const char* org_name, u32 height);
        void shutdown();
        b32 pollEvents(Event::Container& event);
        void swap();
        b32 shouldQuit();
        void executeDrawCalls();
        std::vector<const Draw::Context*>& getDrawQueue();
        void delay(u32 count);
    };
    static const glm::vec2 getViewportSize();
    static const glm::vec2 getMousePos();
    static const std::filesystem::path& getPrefPath();
    static const f32 getViewportScaler();
    static const std::string& getDatPath();
    static void errorMessageBox(const char* title, const char* msg);
    static void cautionMessageBox(const char* title, const char* msg);
    static void infoMessageBox(const char* title, const char* msg);
    static const b32 errorOptionBox(const char* title, const char* msg, b32& res);
    static const b32 cautionOptionBox(const char* title, const char* msg, b32& res);
    static const b32 infoOptionBox(const char* title, const char* msg, b32& res);
private:
    static std::vector<const Draw::Context*> draw_queue;
    static SDL_Window* window;
    static SDL_GLContext context;
    static f32 viewport_scaler;
    static u32 target_height;
    static b32 should_close;
    static std::filesystem::path pref_path;
    static u32 mat_ubo;
};