//
// Created by Feed on 6/19/2020.
//

#include "editor.h"

#include <filesystem>

Editor::Editor(Widget* parent, CacheBank& cache) : Widget(parent), m_lines
        {
            Label(this), "", nullptr,
            Label(this), "", nullptr,
            Label(this), "", nullptr
        }
        {

    m_project = Project(cache);

    anchor = Anchor::Center;
    m_size.x = 700;
    m_font_pixel_height = 36;
    offset = {0.0f, 0.0f};

    m_font = Font::Cache::fetch("editor");

    m_next_line = &m_lines[0];
    m_active_line = &m_lines[1];
    m_prev_line = &m_lines[2];

    m_next_line->label.setWidth(m_size.x);
    m_active_line->label.setWidth(m_size.x);
    m_prev_line->label.setWidth(m_size.x);

    m_prev_line->prev = m_active_line;
    m_active_line->prev = m_next_line;
    m_next_line->prev = m_prev_line;

    m_prev_line->value = m_project.getLastLine();
    m_prev_line->label.setValue(m_prev_line->value, m_font, m_font_pixel_height);
    m_prev_line->label.setAlpha(0.50f);

}

Draw::RedrawFlag Editor::update(f64 delta) {
    static f64 accumulator = 0.0;
    if(m_line_lerp_scaler != 1.0f) {
        accumulator += delta;
        if(accumulator >= m_lint_lerp_time) {
            m_line_lerp_scaler = 1.0f;
            accumulator = 0.0;
        } else {
            // Should be in a header eventually
            f32 t = (accumulator/m_lint_lerp_time);
            f32 m = t-1.0f;
            f32 k = 1.70158;
            m_line_lerp_scaler = 1.0f + m*m*(  m*(k+1) + k);
            accumulator += delta;
        }

        m_active_line->label.offset.y = ((m_font_pixel_height / getSize().y) * getScale() * -1) +
                ((m_font_pixel_height / getSize().y) * getScale() *  m_line_lerp_scaler);
        m_active_line->label.setAlpha(m_line_lerp_scaler);
        m_prev_line->label.offset.y = ((m_font_pixel_height / getSize().y) * getScale() *  m_line_lerp_scaler);
        m_prev_line->label.setAlpha(1.0f - (0.5f * m_line_lerp_scaler));
        if(m_line_lerp_scaler == 1.0f) {
            m_next_line->label.offset.y = ((m_font_pixel_height / getSize().y) * getScale() * -1);
            m_next_line->label.setAlpha(0.0f);
        } else {
            m_next_line->label.offset.y = ((m_font_pixel_height / getSize().y) * getScale()) + ((m_font_pixel_height / getSize().y) * getScale() * m_line_lerp_scaler);
            m_next_line->label.setAlpha(0.5f * 1.0f - m_line_lerp_scaler);
        }
        return true;
    }
    return false;
}

Draw::RedrawFlag Editor::onTextInput(const Event::TextInput& text) {

    m_active_line->value += text.value;
    if(m_font->calcStringPixelWidth(m_active_line->value, m_font_pixel_height) > m_active_line->label.getSize().x) {
        if(m_active_line->value[m_active_line->value.size() - 1] != ' ') {
            static std::string buffer;
            u32 index = 0;
            while(m_active_line->value.at(m_active_line->value.size() - 1 - index) != ' ') {
                ++index;
                if(index == m_active_line->value.size()) {
                    break;
                }
            }
            buffer = m_active_line->value.substr(m_active_line->value.size() - index, index);
            m_active_line->value.erase(m_active_line->value.size() - index, index);
            m_project.pushLine(m_active_line->value);
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);

            m_prev_line = m_prev_line->prev;
            m_active_line = m_active_line->prev;
            m_next_line = m_next_line->prev;
            m_line_lerp_scaler = 0.0f;

            m_active_line->value = buffer;
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_active_line->label.offset.y = 0.0;
        } else {
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_project.pushLine(m_active_line->value);

            m_prev_line = m_prev_line->prev;
            m_active_line = m_active_line->prev;
            m_next_line = m_next_line->prev;
            m_line_lerp_scaler = 0.0f;

            m_active_line->value = "";
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_active_line->label.offset.y = 0.0;
        }
    } else {
        m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
        //m_active_line->label.setAlpha(1.0f);
    }
    return true;
}

Draw::RedrawFlag Editor::onMacro(const Event::Macro& macro) {
    if(macro == Event::Macro::Backspace && !m_active_line->value.empty()) {
        m_active_line->value.pop_back();
        m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
        m_active_line->label.setAlpha(1.0f);
        return true;
    } else if(macro == Event::Macro::Enter) {
        if(!m_active_line->value.empty()) {
            m_project.completeParagraph();

            m_prev_line = m_prev_line->prev;
            m_active_line = m_active_line->prev;
            m_next_line = m_next_line->prev;
            m_line_lerp_scaler = 0.0f;

            m_active_line->value = "";
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_active_line->label.setAlpha(1.0f);
            m_active_line->label.offset.y = 0.0;
            return true;
        }
    } else if(macro == Event::Macro::Save) {
        m_project.save();
    } else if(macro == Event::Macro::Open) {
        m_project.open();
        m_prev_line->value = m_project.getLastLine();
        m_prev_line->label.setValue(m_prev_line->value, m_font, m_font_pixel_height);
        m_prev_line->label.setAlpha(0.33f);
        m_active_line->value = "";
        m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
        m_active_line->label.setAlpha(1.0f);
    } else if(macro == Event::Macro::Export) {
        m_project.exportToTXT();
    }
    return false;
}

Draw::RedrawFlag Editor::onMouseClick(Event::MouseClick mouse_click) {
    return false;
}

void Editor::draw(Draw::CallQueue& draw_buffer, f32 scale) {

    m_size.y = m_parent->getSize().y;
    if(setScaleAndReportChange(scale)) {
        m_draw_size = {m_size.x * getScale(), m_size.y};
    }

    m_next_line->label.draw(draw_buffer, scale);
    m_active_line->label.draw(draw_buffer, scale);
    m_prev_line->label.draw(draw_buffer, scale);

    debugViewUpdate();
    debugViewDraw(draw_buffer);

}