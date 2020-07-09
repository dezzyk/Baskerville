//
// Created by Feed on 6/19/2020.
//

#include "editor.h"

#include <filesystem>

Editor::Editor(Widget* parent, CacheBank& cache) : Widget(parent), m_lines
        {
                Label(this), "", nullptr,
                Label(this), "", nullptr,
                Label(this), "", nullptr,
                Label(this), "", nullptr,
                Label(this), "", nullptr,
                Label(this), "", nullptr
        } {

    m_project;

    anchor = Anchor::Center;
    size.x = 800;
    size.y = getParent()->getSize().y;
    unscaled_width = true;
    m_font_pixel_height = 30;
    offset = {0.0f, 0.0f};

    m_font = Font::Cache::fetch("editor");

    m_next_line = &m_lines[0];
    m_active_line = &m_lines[1];
    m_prev_line_1 = &m_lines[2];
    m_prev_line_2 = &m_lines[3];
    m_prev_line_3 = &m_lines[4];
    m_prev_line_4 = &m_lines[5];

    m_next_line->label.setWidth(size.x);
    m_active_line->label.setWidth(size.x);
    m_prev_line_1->label.setWidth(size.x);
    m_prev_line_2->label.setWidth(size.x);
    m_prev_line_3->label.setWidth(size.x);
    m_prev_line_4->label.setWidth(size.x);

    m_prev_line_4->prev = m_prev_line_3;
    m_prev_line_3->prev = m_prev_line_2;
    m_prev_line_2->prev = m_prev_line_1;
    m_prev_line_1->prev = m_active_line;
    m_active_line->prev = m_next_line;
    m_next_line->prev = m_prev_line_4;

    m_prev_line_4->label.getOffsetRef().y = m_font_pixel_height * 4 + m_line_base_offset;
    m_prev_line_3->label.getOffsetRef().y = m_font_pixel_height * 3 + m_line_base_offset;
    m_prev_line_2->label.getOffsetRef().y = m_font_pixel_height * 2 + m_line_base_offset;
    m_prev_line_1->label.getOffsetRef().y = m_font_pixel_height + m_line_base_offset;
    m_active_line->label.getOffsetRef().y =0.0f + m_line_base_offset;
    m_next_line->label.getOffsetRef().y = m_font_pixel_height * -1 + m_line_base_offset;

}

Draw::RedrawFlag Editor::derivedUpdate(f64 delta) {
    if(m_line_lerp_scaler != 1.0f) {
        m_line_lerp_accumulator += delta;
        if(m_line_lerp_accumulator >= m_line_lerp_time) {
            m_line_lerp_scaler = 1.0f;
            m_line_lerp_accumulator = 0.0;
        } else {
            // Should be in a header eventually
            //f32 t = (m_line_lerp_accumulator / m_line_lerp_time);
            //f32 m = t-1.0f;
            //f32 k = 1.70158;
            //m_line_lerp_scaler = 1.0f + m*m*(  m*(k+1) + k);
            //m_line_lerp_accumulator += delta;

            f32 t = (m_line_lerp_accumulator / m_line_lerp_time);
            f32 m = t-1.0f;
            m_line_lerp_scaler = glm::sqrt( 1.0f-m*m);
            m_line_lerp_accumulator += delta;
        }

        m_active_line->label.getOffsetRef().y = (m_font_pixel_height * -1) + (m_font_pixel_height * m_line_lerp_scaler) + m_line_base_offset;
        m_prev_line_1->label.getOffsetRef().y = (m_font_pixel_height * m_line_lerp_scaler) + m_line_base_offset;
        m_prev_line_2->label.getOffsetRef().y = (m_font_pixel_height) + (m_font_pixel_height * m_line_lerp_scaler) + m_line_base_offset;
        m_prev_line_3->label.getOffsetRef().y = (m_font_pixel_height * 2) + (m_font_pixel_height * m_line_lerp_scaler) + m_line_base_offset;
        m_prev_line_4->label.getOffsetRef().y = (m_font_pixel_height * 3) + (m_font_pixel_height * m_line_lerp_scaler) + m_line_base_offset;
        m_active_line->label.setAlpha(m_line_lerp_scaler);
        m_prev_line_1->label.setAlpha(1.0f - (0.2f * m_line_lerp_scaler));
        m_prev_line_2->label.setAlpha(0.8f - (0.2f * m_line_lerp_scaler));
        m_prev_line_3->label.setAlpha(0.6f - (0.2f * m_line_lerp_scaler));
        m_prev_line_4->label.setAlpha(0.4f - (0.2f * m_line_lerp_scaler));
        if(m_line_lerp_scaler == 1.0f) {
            m_next_line->label.getOffsetRef().y = (m_font_pixel_height * -1) + m_line_base_offset;
            m_next_line->label.setAlpha(0.0f);
        } else {
            m_next_line->label.getOffsetRef().y = (m_font_pixel_height * 4) + (m_font_pixel_height * m_line_lerp_scaler) + m_line_base_offset;
            m_next_line->label.setAlpha(0.2f - (0.2f * m_line_lerp_scaler));
        }
        return true;
    }
    return false;
}

Draw::RedrawFlag Editor::onTextInput(const Event::TextInput& text) {

    // Brute-force af impl for capitalizing starts or paragraphs and sentences.
    std::string additive = text.value;
    if(!m_active_line->value.compare("        ")) {
        m_active_line->value += std::toupper(additive.back());
    } else if (!m_active_line->value.empty()) {
        if(m_active_line->value.back() == '.') {
            if(m_active_line->value.size() >= 2 && m_active_line->value[m_active_line->value.size() - 2] != '.') {
                if(additive.back() != ' ') {
                    if(additive.back() != '.') {
                        m_active_line->value += " ";
                    }
                    m_active_line->value += std::toupper(additive.back());
                } else {
                    m_active_line->value += additive.back();
                }
            } else {
                if(additive.back() != ' ') {
                    if(additive.back() != '.') {
                        m_active_line->value += " ";
                    }
                }
                m_active_line->value += additive.back();
            }
        } else if (m_active_line->value.size() >= 3) {
            if(m_active_line->value.back() == ' ' && m_active_line->value[m_active_line->value.size() - 2] == '.' && m_active_line->value[m_active_line->value.size() - 3] != '.') {
                m_active_line->value += std::toupper(additive.back());
            } else {
                m_active_line->value += additive.back();
            }
        } else {
            m_active_line->value += additive.back();
        }
    } else {
        m_active_line->value += additive.back();
    }

    //
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

            m_prev_line_4 = m_prev_line_4->prev;
            m_prev_line_3 = m_prev_line_3->prev;
            m_prev_line_2 = m_prev_line_2->prev;
            m_prev_line_1 = m_prev_line_1->prev;
            m_active_line = m_active_line->prev;
            m_next_line = m_next_line->prev;
            m_line_lerp_scaler = 0.0f;
            m_line_lerp_accumulator = 0.0;

            m_active_line->value = buffer;
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_active_line->label.getOffsetRef().y = 0.0;
        } else {
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_project.pushLine(m_active_line->value);

            m_prev_line_4 = m_prev_line_4->prev;
            m_prev_line_3 = m_prev_line_3->prev;
            m_prev_line_2 = m_prev_line_2->prev;
            m_prev_line_1 = m_prev_line_1->prev;
            m_active_line = m_active_line->prev;
            m_next_line = m_next_line->prev;
            m_line_lerp_scaler = 0.0f;
            m_line_lerp_accumulator = 0.0;

            m_active_line->value = "";
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_active_line->label.getOffsetRef().y = 0.0;
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
        if(m_active_line->value.compare("        ")) {
            //m_project.completeParagraph();

            m_prev_line_4 = m_prev_line_4->prev;
            m_prev_line_3 = m_prev_line_3->prev;
            m_prev_line_2 = m_prev_line_2->prev;
            m_prev_line_1 = m_prev_line_1->prev;
            m_active_line = m_active_line->prev;
            m_next_line = m_next_line->prev;
            m_line_lerp_scaler = 0.0f;
            m_line_lerp_accumulator = 0.0;

            m_active_line->value = "        ";
            m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
            m_active_line->label.getOffsetRef().y = 0.0;
            return true;
        }
    } else if(macro == Event::Macro::Save) {
        m_project.save();
    } else if(macro == Event::Macro::Open) {
        m_project.open();
        //m_prev_line->value = m_project.getLastLine();
        //m_prev_line->label.setValue(m_prev_line->value, m_font, m_font_pixel_height);
        //m_prev_line->label.setAlpha(0.33f);
        m_active_line->value = "";
        m_active_line->label.setValue(m_active_line->value, m_font, m_font_pixel_height);
        m_active_line->label.setAlpha(1.0f);
        return true;
    } else if(macro == Event::Macro::Export) {
        m_project.exportToTXT();
    }
    return false;
}

Draw::RedrawFlag Editor::onMouseClick(Event::MouseClick mouse_click) {
    return false;
}

void Editor::derivedDraw(Draw::Queue& queue) {

    size.y = getParent()->getSize().y;

    m_next_line->label.draw(queue);
    m_active_line->label.draw(queue);
    m_prev_line_1->label.draw(queue);
    m_prev_line_2->label.draw(queue);
    m_prev_line_3->label.draw(queue);
    m_prev_line_4->label.draw(queue);

}