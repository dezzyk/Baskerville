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
    Draw::RedrawFlag update(f64 delta) {
        return derivedUpdate(delta);
    }
    void draw(Draw::CallQueue& draw_buffer, f32 scale) {
        m_scale = scale;
        derivedDraw(draw_buffer, scale);
        debugViewUpdate();
        debugViewDraw(draw_buffer);
    }
    b32 pointIntersect(glm::vec2 pos);
    const glm::vec2& getSize() const;
    const Widget* getParent() const;

protected:
    const f32& getScale();
    glm::vec2 calcDrawPos();
    glm::vec2 calcDrawSize();

    glm::vec2 size = {1.0f, 1.0f };
    b32 unscaled_width = false;
    b32 unscaled_height = false;
    glm::vec2 offset = {0.0f, 0.0f};
    Anchor anchor = Anchor::TopLeft;

private:
    virtual Draw::RedrawFlag derivedUpdate(f64 delta) = 0;
    virtual void derivedDraw(Draw::CallQueue& draw_buffer, f32 scale) = 0;
    void debugViewUpdate();
    void debugViewDraw(Draw::CallQueue& draw_buffer);

    f32 m_scale = 1.0f;
    //glm::vec2 m_draw_size = m_size;
    Widget* m_parent = nullptr;
    struct {
        Draw::Context context;
        const Shader* shader = nullptr;
        b32 draw_attempted = false;
    } m_debug_draw;

};