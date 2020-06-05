//
// Created by Feed on 5/31/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "font.h"
#include "shader.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

class FontRenderTest : public Widget {
public:
    FontRenderTest(Widget& parent);
    void onCodepoint(const Event::Codepoint& codepoint);
    void onMacro(const Event::Macro& macro);
    void onWindowResize(const Event::WindowResize& window_resize);
    void draw(Draw::CallQueue& draw_buffer);
private:
    glm::vec3 m_canvas_color = { 1.0f, 1.0f, 1.0f};
    const Font* m_font = nullptr;
    const Shader* m_shader = nullptr;
    Draw::Handles m_draw_handles;
    u32 m_codepoint = 33;
};