//
// Created by Feed on 5/29/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "anchor_test_box.h"

#include "glm/glm.hpp"

class Root : public Widget {
public:
    Root();
    void onCodepoint(const Event::Codepoint& codepoint);
    void onMacro(const Event::Macro& macro);
    void onWindowResize(const Event::WindowResize& window_resize);
    void draw(Window::DrawBuffer& draw_buffer);
private:
    AnchorTestBox test;
    glm::vec3 m_canvas_color = { 1.0f, 1.0f, 1.0f};
};
