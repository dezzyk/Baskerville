//
// Created by Feed on 5/29/2020.
//

#include "root.h"

#include "gl_err.h"

// NOTE: ALL child widget's must be instantiated shorthand like this
Root::Root() : m_editor(*this){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Root::onCodepoint(const Event::Codepoint& codepoint) {
    m_editor.onCodepoint(codepoint);
}

void Root::onMacro(const Event::Macro& macro) {
    m_editor.onMacro(macro);
}

void Root::onWindowResize(const Event::WindowResize& window_resize) {
    m_size = { window_resize.x, window_resize.y};
    m_editor.onWindowResize(window_resize);
}

void Root::draw(Draw::CallQueue& draw_buffer) {
    debugDraw(draw_buffer);
    m_editor.draw(draw_buffer);
}
