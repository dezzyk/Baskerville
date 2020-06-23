//
// Created by Feed on 6/19/2020.
//

#include "editor.h"

#include <filesystem>

Editor::Editor(Widget* parent) : Widget(parent) {
    anchor = Anchor::Center;
    m_size.x = 700;
    m_font_pixel_height = 36;
    offset = {0.0f, 0.0f};

    m_lines.reserve(5);
    for(int i = 0; i < 5; ++i) {
        m_lines.push_back({Label(this), ""});
        m_lines[i].label.setWidth(m_size.x);
    }

    cur_line = &m_lines[m_cur_line_index];
    prev_line = &m_lines[4];

    m_font = Font::Cache::fetch("editor");

    prev_line->value = Project::getLastLine();
    prev_line->label.setValue(prev_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 0.33f});

}

void Editor::update() {

}

void Editor::onCodepoint(const Event::Codepoint& codepoint) {

    cur_line = &m_lines[m_cur_line_index];
    cur_line->value += (char)codepoint.value;
    if(m_font->calcStringPixelWidth(cur_line->value, m_font_pixel_height) > cur_line->label.getSize().x) {
        if(cur_line->value[cur_line->value.size() - 1] != ' ') {
            static std::string buffer;
            u32 index = 0;
            while(cur_line->value.at(cur_line->value.size() - 1 - index) != ' ') {
                ++index;
                if(index == cur_line->value.size()) {
                    break;
                }
            }
            buffer = cur_line->value.substr(cur_line->value.size() - index, index);
            cur_line->value.erase(cur_line->value.size() - index, index);
            Project::pushLine(cur_line->value);
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 0.33f});
            //cur_line->label.offset.y = (m_font_pixel_height / getSize().y) * getScale();
            prev_line = cur_line;
            ++m_cur_line_index;
            if(m_cur_line_index > 4) {
                m_cur_line_index = 0;
            }
            cur_line = &m_lines[m_cur_line_index];
            cur_line->value = buffer;
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
            cur_line->label.offset.y = 0.0;
        } else {
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 0.33f});
            Project::pushLine(cur_line->value);
            //cur_line->label.offset.y = (m_font_pixel_height / getSize().y) * getScale();
            prev_line = cur_line;
            ++m_cur_line_index;
            if(m_cur_line_index > 4) {
                m_cur_line_index = 0;
            }
            cur_line = &m_lines[m_cur_line_index];
            cur_line->value = "";
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
            cur_line->label.offset.y = 0.0;
        }
    } else {
        cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
    }
}

void Editor::onTextInput(const Event::TextInput& text) {

    cur_line = &m_lines[m_cur_line_index];
    cur_line->value += text.value;
    if(m_font->calcStringPixelWidth(cur_line->value, m_font_pixel_height) > cur_line->label.getSize().x) {
        if(cur_line->value[cur_line->value.size() - 1] != ' ') {
            static std::string buffer;
            u32 index = 0;
            while(cur_line->value.at(cur_line->value.size() - 1 - index) != ' ') {
                ++index;
                if(index == cur_line->value.size()) {
                    break;
                }
            }
            buffer = cur_line->value.substr(cur_line->value.size() - index, index);
            cur_line->value.erase(cur_line->value.size() - index, index);
            Project::pushLine(cur_line->value);
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 0.33f});
            //cur_line->label.offset.y = (m_font_pixel_height / getSize().y) * getScale();
            prev_line = cur_line;
            ++m_cur_line_index;
            if(m_cur_line_index > 4) {
                m_cur_line_index = 0;
            }
            cur_line = &m_lines[m_cur_line_index];
            cur_line->value = buffer;
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
            cur_line->label.offset.y = 0.0;
        } else {
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 0.33f});
            Project::pushLine(cur_line->value);
            //cur_line->label.offset.y = (m_font_pixel_height / getSize().y) * getScale();
            prev_line = cur_line;
            ++m_cur_line_index;
            if(m_cur_line_index > 4) {
                m_cur_line_index = 0;
            }
            cur_line = &m_lines[m_cur_line_index];
            cur_line->value = "";
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
            cur_line->label.offset.y = 0.0;
        }
    } else {
        cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
    }
}

void Editor::onMacro(const Event::Macro& macro) {
    //auto& line = m_lines[m_cur_line_index];
    if(macro == Event::Macro::Backspace && !cur_line->value.empty()) {
        cur_line->value.pop_back();
        cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
    }
    else if(macro == Event::Macro::Enter) {
        if(!cur_line->value.empty()) {
            Project::completeParagraph();
            prev_line = cur_line;
            prev_line->label.setValue(prev_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 0.33f});
            //prev_line->label.offset.y = (m_font_pixel_height / getSize().y) * getScale();
            ++m_cur_line_index;
            if (m_cur_line_index > 4) {
                m_cur_line_index = 0;
            }
            cur_line = &m_lines[m_cur_line_index];
            cur_line->value = "";
            cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
            cur_line->label.offset.y = 0.0;
        }
    } else if(macro == Event::Macro::Save) {
        Project::save();
    } else if(macro == Event::Macro::Open) {
        Project::open();
        prev_line->value = Project::getLastLine();
        prev_line->label.setValue(prev_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 0.33f});
        cur_line->value = "";
        cur_line->label.setValue(cur_line->value, m_font, m_font_pixel_height, {0.0f, 0.0f, 0.0f, 1.0f});
    }
}

void Editor::onMouseClick(Event::MouseClick mouse_click) {

}

void Editor::draw(Draw::CallQueue& draw_buffer, f32 scale) {

    m_size.y = m_parent->getSize().y;
    prev_line->label.offset.y = (m_font_pixel_height / getSize().y) * getScale();
    if(setScaleAndReportChange(scale)) {
        m_draw_size = {m_size.x * getScale(), m_size.y};
    }

    cur_line->label.draw(draw_buffer, scale);
    prev_line->label.draw(draw_buffer, scale);

    debugViewUpdate();
    debugViewDraw(draw_buffer);
}