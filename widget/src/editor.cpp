//
// Created by Feed on 6/4/2020.
//

#include "editor.h"

Editor::Editor() {}

Editor::Editor(Widget* parent) : Widget(parent) {
    m_anchor = Widget::Anchor::Top;
    m_size.x = 800;
    m_size.y = m_parent->getSize().y;
}

void Editor::update() {

}

void Editor::onCodepoint(const Event::Codepoint& codepoint) {

}

void Editor::onMacro(const Event::Macro& macro) {

}

void Editor::onMouseClick(Event::MouseClick mouse_click) {

}

void Editor::onWindowResize() {
    m_size.y = m_parent->getSize().y;
    m_draw_size = {m_size.x * Platform::getGlobalScaler(), m_size.y};
    debugViewUpdate();

}

void Editor::draw(Draw::CallQueue& draw_buffer) {
    debugViewDraw(draw_buffer);
}