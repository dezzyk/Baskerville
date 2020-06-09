//
// Created by Feed on 6/4/2020.
//

#include "editor.h"

Editor::Editor() {}

Editor::Editor(Widget* parent) : Widget(parent), m_chaper_number(this) {
    m_anchor = Widget::Anchor::Top;
    m_size.x = 800;
    m_size.y = m_parent->getSize().y;
    m_chaper_number.setValue('1');
}

void Editor::update() {
    m_chaper_number.update();
}

void Editor::onCodepoint(const Event::Codepoint& codepoint) {
    m_chaper_number.onCodepoint(codepoint);
}

void Editor::onMacro(const Event::Macro& macro) {
    m_chaper_number.onMacro(macro);
}

void Editor::onMouseClick(Event::MouseClick mouse_click) {
    if(m_chaper_number.pointIntersect(mouse_click.pos)) {
        std::cout << "Hit" << std::endl;
    } else {
        std::cout << "Miss" << std::endl;
    }
}

void Editor::onWindowResize() {
    m_size.y = m_parent->getSize().y;
    m_draw_size = {m_size.x * Platform::getGlobalScaler(), m_size.y};
    m_draw_offset = m_offset * Platform::getGlobalScaler();
    debugViewUpdate();
    std::cout << "Upload" << std::endl;
    m_chaper_number.onWindowResize();
}

void Editor::draw(Draw::CallQueue& draw_buffer) {
    m_chaper_number.draw(draw_buffer);
    debugViewDraw(draw_buffer);
}