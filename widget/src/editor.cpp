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
    auto& line = m_lines[m_active_line_index];
    line.value += (char)codepoint.value;
    line.label.setValue(line.value, 24, {0.0f, 0.0f, 0.0f, 1.0f});
    if(line.label.getPixelWidth() > line.label.getSize().x) {
        static std::string copy;
        static std::string buffer;
        copy = line.value;
        buffer = "";
        u32 index = 0;
        while(line.label.calcPixelWidth(copy, 24) > line.label.getSize().x) {
            ++index;
            copy.pop_back();
        }
        while(line.value.at(line.value.size() - 1 - index) != ' ') {
            ++index;
        }
        ++m_active_line_index;
        if(m_active_line_index > 4) {
            m_active_line_index = 0;
        }
        auto& next_line = m_lines[m_active_line_index];
        next_line.value = line.value.substr(line.value.size() -index, index);
        next_line.label.setValue(next_line.value, 24, {0.0f, 0.0f, 0.0f, 1.0f});
    }
    std::cout << line.label.getPixelWidth() << std::endl;
}

void Editor::onMacro(const Event::Macro& macro) {
    auto& line = m_lines[m_active_line_index];
    if(macro == Event::Macro::Backspace && !line.value.empty()) {
        line.value.pop_back();
        line.label.setValue(line.value, 24, {0.0f, 0.0f, 0.0f, 1.0f});
        std::cout << line.label.getPixelWidth() << std::endl;
    }
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