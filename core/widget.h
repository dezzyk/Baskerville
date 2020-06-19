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
    virtual void draw(Draw::CallQueue& draw_buffer, f32 scale);
    b32 pointIntersect(glm::vec2 pos);
    const glm::vec2& getSize() const;
    const glm::vec2& getOffset() const;
    const Anchor& getAnchor() const;
protected:
    void debugViewUpdate();
    void debugViewDraw(Draw::CallQueue& draw_buffer);
    const glm::vec2& getDrawSize();
    const f32& getScale();
    b32 setScaleAndReportChange(f32 scale);
    void updateDrawSize();
    glm::vec2 calcDrawPos();
    glm::vec2 m_size = {1.0f, 1.0f };
    glm::vec2 m_offset = {0.0f, 0.0f};
    Widget* m_parent = nullptr;
    b32 m_offset_normalized = false;
    Anchor m_anchor = Anchor::TopLeft;
private:
    glm::vec2 m_draw_size = m_size;
    f32 m_scale = 0.0f;
    struct {
        Draw::Context context;
        const Shader* shader = nullptr;
        b32 draw_attempted = false;
    } m_debug_draw;

};