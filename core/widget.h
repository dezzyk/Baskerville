//
// Created by Feed on 5/26/2020.
//

#pragma once

#include "platform.h"
#include "draw.h"
#include "common.h"
#include "event.h"
#include "gl_err.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

class Widget {
public:
    Widget() {}
    Widget(Widget* parent);
    Widget(Widget&& other) noexcept;
    Widget(const Widget&) =delete;
    Widget& operator=(const Widget&) =delete;
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
    virtual void update();
    virtual void onCodepoint(const Event::Codepoint& codepoint);
    virtual void onMacro(const Event::Macro& macro);
    virtual void onMouseClick(Event::MouseClick mouse_click);
    virtual void onWindowResize();
    virtual void draw(Draw::CallQueue& draw_buffer);
    virtual void setParent(Widget* parent);
    b32 pointIntersect(glm::vec2 pos);
    glm::vec2 getSize() const;
    glm::vec2 getOffset() const;
    Anchor getAnchor() const;
protected:
    void debugViewUpdate();
    void debugViewDraw(Draw::CallQueue& draw_buffer);
    glm::vec2 calcViewportPos();
    glm::vec2 m_size = {1.0f, 1.0f };
    glm::vec2 m_offset = {0.0f, 0.0f};
    glm::vec2 m_draw_size = m_size;
    glm::vec2 m_draw_offset = m_offset;
    Widget* m_parent = nullptr;
    b32 m_offset_normalized = false;
    Anchor m_anchor = Anchor::TopLeft;
    struct {
        Draw::Context context;
        const Shader* shader = nullptr;
        b32 draw_attempted = false;
    } m_debug_draw;

};