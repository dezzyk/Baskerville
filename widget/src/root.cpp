//
// Created by Feed on 5/29/2020.
//

#include "root.h"

#include "gl_err.h"

Root::Root() {
    glClearColor(m_canvas_color.r, m_canvas_color.g, m_canvas_color.b, 1.0f);
    test = AnchorTestBox(*this);
}

void Root::onCodepoint(const Event::Codepoint& codepoint) {

}

void Root::onMacro(const Event::Macro& macro) {
    test.onMacro(macro);
}

void Root::onWindowResize(const Event::WindowResize& window_resize) {
    m_size = { window_resize.x, window_resize.y};
    //m_offset = { (1.0f - (m_size.x / window_resize.x)) / 2, (1.0f - (m_size.y / window_resize.y)) / 2};
    test.onWindowResize(window_resize);
}

void Root::draw(Window::DrawBuffer& draw_buffer) {
    debugDraw(draw_buffer);
    test.draw(draw_buffer);
}
