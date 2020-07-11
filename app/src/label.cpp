//
// Created by Feed on 6/14/2020.
//

#include "label.h"

Label::Label(Widget* parent) : Widget(parent) {
    anchor = Widget::Anchor::Center;
    m_draw_context = Draw::Context("msdf");
    size = {0.0f, 0.0f};
    unscaled_width = true;
}

void Label::setValue(const std::string& value, const Font* font, u32 pixel_height) {

    //m_color = color;
    m_font = font;
    if(m_font != nullptr) {

        m_pixel_height = pixel_height;
        size.y = m_font->getPixelHeight() * (m_pixel_height / m_font->getPixelHeight());

        if (!value.empty()) {

            static std::vector<Draw::Quad> boxes;
            boxes.resize(0);

            i32 xpos = 0;
            for (int i = 0; i < value.size(); ++i) {

                i32 advance;
                i32 lsb = 0;
                m_font->getCodepointAdvance(m_pixel_height, value[i], advance, lsb);

                // Calc an offset for the glyph to orient it in accordence to the font metrics since they always render from the center.
                int xoffset = advance / 2;
                if (i == 0) {
                    xoffset -= lsb; // lsb is always negative
                }

                if (value[i] != ' ' && m_font->getCodepointLayer((u32)value[i]) != -1) {

                    Draw::Quad box;
                    box.setColor({1.0f, 1.0f, 1.0f, 1.0f});
                    box.setUVDepth(m_font->getCodepointLayer((u32)value[i]));

                    glm::mat4 model = glm::mat4(1.0f);
                    glm::vec2 bounding_box_size = m_font->getBoundingBoxSize();
                    model = glm::translate(model, glm::vec3(xpos + xoffset, 0.0f, 0.0f));
                    model = glm::scale(model, glm::vec3(bounding_box_size.x, bounding_box_size.y, 0.0f));

                    box *= model;

                    boxes.push_back(box);

                }

                xpos += advance;
                int kern = 0;
                if (i < value.size() - 1) {
                    xpos += m_font->getKernAdvance(m_pixel_height, value[i], value[i + 1]);
                }

                if(xpos * m_font->calcScale(m_pixel_height) > size.x) {
                    break;
                }

            }

            if (!boxes.empty()) {
                m_draw_context.quadUpload(boxes);
            } else {
                m_draw_context.clear();
            }

            //m_size.x = xpos * m_font->calcScale(m_pixel_height);

        } else {
            m_draw_context.clear();
        }

    }

}

void Label::setColor(glm::vec3 color) {
    m_color = {color.r,  color.g, color.b, m_color.a};
}

void Label::setAlpha(f32 alpha) {
    m_color.a = alpha;
}

void Label::derivedDraw(Draw::Queue& queue) {

    glm::vec2 draw_size = calcDrawSize();

    if (m_draw_context.size() > 0) {

        f32 scale = Platform::getViewportScaler();

        glm::vec2 draw_pos = calcDrawPos();
        //glm::vec2 draw_size = calcDrawSize();
        m_draw_context.model = glm::mat4(1.0f);
        m_draw_context.model = glm::translate(m_draw_context.model, glm::vec3(draw_pos.x - draw_size.x / 2, draw_pos.y, 0.0f));
        m_draw_context.model = glm::scale(m_draw_context.model, glm::vec3(m_font->calcScale(m_pixel_height) * scale,
                                            m_font->calcScale(m_pixel_height) * scale, 0.0f));

        m_draw_context.uniforms[0].setValue(Draw::Uniform::ArrayTexture(m_font->getHandle().value(), 0));
        m_draw_context.uniforms[1].setValue(m_color);

        queue.push_back(&m_draw_context);

    }

}

void Label::setWidth(u32 width) {
    size.x = width;
}

glm::vec2& Label::getOffsetRef() {
    return offset;
}