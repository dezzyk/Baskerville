//
// Created by Feed on 6/19/2020.
//

#include "editor.h"

#include <filesystem>

Editor::Editor(Widget* parent) : Widget(parent) {
    m_anchor = Anchor::Center;
    m_size.x = 700;
    m_font_pixel_height = 36;
    m_offset = {0.0f, 0.0f};

    m_lines.reserve(5);
    for(int i = 0; i < 5; ++i) {
        m_lines.push_back({Label(this), ""});
        m_lines[i].label.setWidth(m_size.x);
    }
    m_active_line_index = 0;

    // Temp

    m_project["name"] = "Test Project";
    m_project["word_count"] = 0;
    m_current_paragraph = "";

    //

    m_font = Font::Cache::fetch("editor");
}

void Editor::update() {}

void Editor::onCodepoint(const Event::Codepoint& codepoint) {
    auto& line = m_lines[m_active_line_index];
    line.value += (char)codepoint.value;
    line.label.setValue(line.value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
    if(m_font->calcStringPixelWidth(line.value, m_font_pixel_height) > line.label.getSize().x) {
        static std::string copy;
        //static std::string buffer;
        copy = line.value;
        //buffer = "";
        u32 index = 0;
        while(m_font->calcStringPixelWidth(copy, m_font_pixel_height) > line.label.getSize().x) {
            ++index;
            copy.pop_back();
        }
        while(line.value.at(line.value.size() - 1 - index) != ' ') {
            ++index;
            if(index == line.value.size()) {
                break;
            }
        }
        m_current_paragraph += copy;
        ++m_active_line_index;
        if(m_active_line_index > 4) {
            m_active_line_index = 0;
        }
        auto& next_line = m_lines[m_active_line_index];
        if(index < line.value.size()) {
            next_line.value = line.value.substr(line.value.size() -index, index);
            next_line.label.setValue(next_line.value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
        } else {
            next_line.label.setValue("", m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
        }
    }
}

void Editor::onMacro(const Event::Macro& macro) {
    auto& line = m_lines[m_active_line_index];
    if(macro == Event::Macro::Backspace && !line.value.empty()) {
        line.value.pop_back();
        line.label.setValue(line.value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
    }
    else if(macro == Event::Macro::Enter) {
        if(!m_lines[m_active_line_index].value.empty()) {
            m_project["paragraphs"].push_back(m_current_paragraph);
            m_current_paragraph = "";
            m_lines[m_active_line_index].value = "";
            m_lines[m_active_line_index].label.setValue(m_lines[m_active_line_index].value, m_font, 24, {0.0f, 0.0f, 0.0f, 1.0f});
            ++m_active_line_index;
            if (m_active_line_index > 4) {
                m_active_line_index = 0;
            }
        }
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