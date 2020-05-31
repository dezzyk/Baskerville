//
// Created by Feed on 5/31/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "font.h"
#include "shader.h"

class FontRenderTest : public Widget {
public:
    FontRenderTest();
    FontRenderTest(Widget& parent);
    void onCodepoint(const Event::Codepoint& codepoint);
    void onMacro(const Event::Macro& macro);
    void onWindowResize(const Event::WindowResize& window_resize);
    void draw(Window::DrawBuffer& draw_buffer);
private:
    glm::vec3 m_canvas_color = { 1.0f, 1.0f, 1.0f};
    const Font* m_font = nullptr;
    const Shader* m_shader = nullptr;
    u32 m_codepoint = 33;
};