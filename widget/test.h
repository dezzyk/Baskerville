//
// Created by Feed on 5/30/2020.
//

#pragma once

#include "common.h"
#include "widget.h"

#include "glm/glm.hpp"

class TestWidget : public Widget {
public:
    TestWidget();
    TestWidget(Widget& parent);
    void onCodepoint(const Event::Codepoint& codepoint);
    void onMacro(const Event::Macro& macro);
    void onWindowResize(const Event::WindowResize& window_resize);
    void draw(const Window::Viewport& viewport);
private:
    glm::vec3 m_canvas_color = { 1.0f, 1.0f, 1.0f};
};

