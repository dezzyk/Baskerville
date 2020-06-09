//
// Created by Feed on 6/4/2020.
//

#include "single_char.h"

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

SingleChar::SingleChar() {}

SingleChar::SingleChar(Widget* parent) : Widget(parent) {

    m_font = Font::Cache::fetch("editor");
    m_shader = Shader::Cache::fetch("msdf_draw");
    if(m_shader == nullptr) {
        std::string vert =
#include "shader/msdf_draw.vert"
        ;
        std::string frag =
#include "shader/msdf_draw.frag"
        ;
        m_shader = Shader::Cache::load("msdf_draw", vert, frag);
    }

    m_anchor = Widget::Anchor::Top;
    m_offset = {0.0f, 0.1f};
    m_size = {m_font->getBitmapSize().x * m_font->calcPXScaler(48),
              m_font->getBitmapSize().y * m_font->calcPXScaler(48)};

}

void SingleChar::setValue(u32 codepoint) {

    if(codepoint >= m_font->getStartCodepoint() && codepoint <= m_font->getEndCodepoint()) {

        m_codepoint = codepoint;
        Draw::Box box;
        for (auto &vert : box.vertices) {
            vert.color = {0.0f, 0.0f, 0.0f, 1.0f};
            vert.uv.z = codepoint - m_font->getStartCodepoint();
        }
        m_draw_context.upload(sizeof(Draw::Box), &box);

    }

}

void SingleChar::setValue(char codepoint) {
    setValue((u32)codepoint);
}

void SingleChar::onWindowResize() {
    m_draw_size = m_size * Platform::getGlobalScaler();
    m_draw_offset = m_offset;
    debugViewUpdate();
}

void SingleChar::draw(Draw::CallQueue& draw_buffer) {

    if(m_codepoint.has_value()) {

        glm::vec2 draw_pos = calcViewportPos();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x, draw_pos.y, 0.0f));
        model = glm::scale(model, glm::vec3(m_draw_size.x, m_draw_size.y, 0.0f));

        Draw::Call new_draw;
        new_draw.context = &m_draw_context;
        new_draw.shader = m_shader;
        new_draw.count = 6;
        new_draw.uniforms[0].setValue(model);
        new_draw.uniforms[1].setValue(Draw::Uniform::ArrayTexture(m_font->getHandle().value(), 0));
        draw_buffer.push_back(new_draw);

    }

    debugViewDraw(draw_buffer);

}