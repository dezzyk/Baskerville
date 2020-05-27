//
// Created by Feed on 5/26/2020.
//

#pragma once

#include "common.h"
#include "event.h"
#include "draw.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <iostream>

class Widget {
public:
    Widget();
    ~Widget();
    enum class Anchor {
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };
    struct DrawParams {
        glm::vec2 m_offset = {0.0f, 0.0f};
        Anchor m_anchor = Anchor::Top;
        std::optional<u32> shader;
        std::optional<u32> buffer;
    };
    virtual void handleTextInput(u32 value);
    virtual void handleMacro(Event::Macro value);
    virtual void handleResize(Event::Resize value);
    const DrawParams& getDrawParams();
private:
    DrawParams m_draw_params;
};

inline Widget::Widget() {
    u32 buffer;
    glGenBuffers(1, &buffer);
    if(buffer != 0) {
        m_draw_params.buffer = buffer;
    } else {
        std::cout << "Failed to create buffer for widget" << std::endl;
    }
}

inline Widget::~Widget() {
    if(m_draw_params.buffer.has_value()) {
        glDeleteBuffers(1, &m_draw_params.buffer.value());
        m_draw_params.buffer = std::nullopt;
    }
}


inline const Widget::DrawParams& Widget::getDrawParams() {
    return m_draw_params;
}