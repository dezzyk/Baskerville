//
// Created by Feed on 6/4/2020.
//

#include "editor.h"

Editor::Editor() {}

Editor::Editor(Widget* parent) : Widget(parent), m_chaper_number(this) {
    m_anchor = Widget::Anchor::Top;
    m_size.x = 800;
    m_size.y = m_parent->getSize().y;
}

void Editor::onCodepoint(const Event::Codepoint& codepoint) {

}

void Editor::onMacro(const Event::Macro& macro) {

}

void Editor::onWindowResize(const Event::WindowResize& window_resize) {
    m_size.y = m_parent->getSize().y;
    m_chaper_number.onWindowResize(window_resize);
}

void Editor::draw(Draw::CallQueue& draw_buffer) {
    m_chaper_number.draw(draw_buffer);
    debugDraw(draw_buffer);
}

void Editor::update() {
    Widget::update();
    m_draw_size.y = m_size.y;
    m_chaper_number.update();
}