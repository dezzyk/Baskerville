//
// Created by Feed on 5/31/2020.
//

#include "font_render_test.h"

FontRenderTest::FontRenderTest() {
    m_font = Font::Cache::fetch("editor");
    m_shader = Shader::Cache::fetch("msdf_draw");
    if(m_shader == nullptr) {
        //m_shader = Shader::Cache::load("msdf_draw");
    }
}

FontRenderTest::FontRenderTest(Widget& parent) : Widget(parent) {
    m_font = Font::Cache::fetch("editor");
}

void FontRenderTest::onCodepoint(const Event::Codepoint& codepoint) {
    if(m_font) {
        if (codepoint.value >= m_font->getStartCodepoint() && codepoint.value <= m_font->getEndCodepoint()) {
            m_codepoint = codepoint.value;
        }
    }
}

void FontRenderTest::onMacro(const Event::Macro& macro) {

}

void FontRenderTest::onWindowResize(const Event::WindowResize& window_resize) {

}

void FontRenderTest::draw(Window::DrawBuffer& draw_buffer) {

}