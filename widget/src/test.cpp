//
// Created by Feed on 6/14/2020.
//

#include "test.h"

Test::Test() {}

Test::Test(Widget* parent) : Widget(parent) {
    //m_size = {32, 32};
    m_anchor = Widget::Anchor::Center;
    m_shader = Shader::Cache::fetch("msdf_draw");
    m_font = Font::Cache::fetch("editor");
    //m_value = "Once upon a time at the foot of a great mountain, there was a town where the people known as happy folk lived.";
    m_size = {1344, m_font->getBitmapSize().y };
}

void Test::onWindowResize() {
    m_draw_size = m_size * Platform::getGlobalScaler();
    m_draw_offset = m_offset * Platform::getGlobalScaler();
    debugViewUpdate();
}

void Test::onCodepoint(const Event::Codepoint& codepoint) {
    m_value += (char)codepoint.value;
    updateLabel();
    std::cout << m_value << std::endl;
}
void Test::onMacro(const Event::Macro& macro) {
    if(macro == Event::Macro::Backspace && !m_value.empty()) {
        m_value.pop_back();
        updateLabel();
        /*if(m_value.back() != ' ') {
            m_value.pop_back();
            updateLabel();
        } else {
            m_value.pop_back();
        }*/
        std::cout << m_value << std::endl;
    }
}

void Test::draw(Draw::CallQueue& draw_buffer) {

    if(!m_value.empty()) {

        glm::vec2 draw_pos = calcViewportPos();
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(draw_pos.x - (xpos * m_font->calcScale(24) * Platform::getGlobalScaler()) / 2, draw_pos.y, 0.0f));
        model = glm::translate(model, glm::vec3(draw_pos.x - m_draw_size.x / 2, draw_pos.y, 0.0f));
        model = glm::scale(model, glm::vec3(m_font->calcScale(24) * Platform::getGlobalScaler(),
                                            m_font->calcScale(24) * Platform::getGlobalScaler(), 0.0f));

        Draw::Call new_draw;
        new_draw.context = &m_draw_context;
        new_draw.shader = m_shader;
        new_draw.count = m_draw_context.size();
        new_draw.uniforms[0].setValue(model);
        new_draw.uniforms[1].setValue(Draw::Uniform::ArrayTexture(m_font->getHandle().value(), 0));
        draw_buffer.push_back(new_draw);

    }

    debugViewDraw(draw_buffer);
}

void Test::updateLabel() {

    if(!m_value.empty()) {

        static std::vector<Draw::Box> boxes;
        boxes.resize(0);

        i32 xpos = 0;
        for (int i = 0; i < m_value.size(); ++i) {

            i32 advance;
            i32 lsb = 0;
            m_font->getGlyphAdvance(32, m_value[i], advance, lsb);

            // Calc an offset for the glyph to orient it in accordence to the font metrics since they always render from the center.
            int xoffset = advance / 2;
            if (i == 0) {
                xoffset -= lsb; // lsb is always negative
            }

            if (m_value[i] != ' ') {

                Draw::Box box;
                box.setColor({0.0f, 0.0f, 0.0f, 1.0f});
                box.setUVDepth((int) m_value[i] - m_font->getStartCodepoint());

                glm::mat4 model = glm::mat4(1.0f);
                glm::vec2 bounding_box_size = m_font->getBoundingBoxSize();
                model = glm::translate(model, glm::vec3(xpos + xoffset, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(bounding_box_size.x, bounding_box_size.y, 0.0f));

                box *= model;

                boxes.push_back(box);

            }

            xpos += advance;
            int kern = 0;
            if (i < m_value.size() - 1) {
                xpos += m_font->getKernAdvance(32, m_value[i], m_value[i + 1]);
            }

        }

        m_draw_context.boxUpload(boxes);

    }

}