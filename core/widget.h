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
    virtual void handleCodepoint(Event::Codepoint codepoint) = 0;
    virtual void handleMacro(Event::Macro macro) = 0;
    virtual void handleResize(Event::Resize resize) = 0;
    virtual void draw() = 0;
    const glm::vec4& getFrame();
    const glm::vec2& getOffset();
    const b32 offsetNormalized();
    const Anchor& getAnchor();
    const Widget* const getParent();

private:
    glm::vec4 m_frame = { 0.0f, 0.0f, 0.0f, 0.0f};
    glm::vec2 m_offset = {0.0f, 0.0f};
    b32 m_offset_normalized = false;
    Anchor m_anchor = Anchor::Center;
    Widget* m_parent = nullptr;

};

inline Widget::Widget() {}
inline Widget::Widget(Widget& widget) : m_parent(&widget) {}
inline Widget::~Widget() {}
inline const glm::vec4& Widget::getFrame() { return m_frame; }
inline const glm::vec2& Widget::getOffset() { return m_offset; }
inline const b32 Widget::offsetNormalized() { return m_offset_normalized; }
inline const Widget::Anchor& Widget::getAnchor() { return m_anchor; }
inline const Widget* const Widget::getParent() { return m_parent; }