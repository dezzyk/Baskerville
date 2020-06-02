//
// Created by Feed on 5/26/2020.
//

#pragma once

#include "common.h"
#include "window.h"
#include "event.h"
#include "gl_err.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <iostream>

class Widget {
public:
    Widget() {}
    Widget(Widget& widget);
    ~Widget();
    enum class Anchor {
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center
    };
    virtual void onCodepoint(const Event::Codepoint& codepoint) = 0;
    virtual void onMacro(const Event::Macro& macro) = 0;
    virtual void onWindowResize(const Event::WindowResize& resize) = 0;
    virtual void draw(Window::DrawBuffer& draw_buffer) = 0;

protected:
    void debugDraw(Window::DrawBuffer& draw_buffer);
    glm::vec2 calcDrawPos();
    glm::vec2 m_size = { 0.0f, 0.0f };
    glm::vec2 m_offset = {0.0f, 0.0f};
    Widget* m_parent = nullptr;
    b32 m_offset_normalized = false;
    Anchor m_anchor = Anchor::TopLeft;
    struct {
        const Shader* shader = nullptr;
        std::optional<u32> vao;
        std::optional<u32> vbo;
        b32 draw_attempted = false;
    } m_debug_draw_params;

};