//
// Created by Feed on 6/19/2020.
//

#include "editor.h"

Editor::Editor(Widget* parent) : Widget(parent) {
    m_anchor = Anchor::Center;
    m_size.x = 1000;
    m_offset = {0.0f, 0.0f};

    m_lines.reserve(5);
    for(int i = 0; i < 5; ++i) {
        m_lines.push_back({Label(this), ""});
    }
    m_active_line_index = 0;
}

void Editor::update() {}

void Editor::onCodepoint(const Event::Codepoint& codepoint) {

}

void Editor::onMacro(const Event::Macro& macro) {

}

void Editor::onMouseClick(Event::MouseClick mouse_click) {

}

void Editor::draw(Draw::CallQueue& draw_buffer, f32 scale) {

    m_size.y = m_parent->getSize().y;
    if(setScaleAndReportChange(scale)) {
        m_draw_size = {m_size.x * getScale(), m_size.y};
    }

    auto& line = m_lines[m_active_line_index];
    line.label.draw(draw_buffer, scale);

    debugViewUpdate();
    debugViewDraw(draw_buffer);
}