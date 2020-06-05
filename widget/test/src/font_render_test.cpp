//
// Created by Feed on 5/31/2020.
//

#include "test/font_render_test.h"

#include "glm/gtc/matrix_transform.hpp"

FontRenderTest::FontRenderTest(Widget& parent) : Widget(parent) {
    m_anchor = Widget::Anchor::TopRight;
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

    if(m_font != nullptr) {
        glm::vec2 bitmap_size = m_font->getBitmapSize();
        m_size = {bitmap_size.x, bitmap_size.y};
        generateDrawHandles(m_draw_handles);
    }
}

void FontRenderTest::onCodepoint(const Event::Codepoint& codepoint) {
    if(m_font) {
        u32 start = m_font->getStartCodepoint();
        u32 end = m_font->getEndCodepoint();
        if (codepoint.value >= start && codepoint.value <= end) {
            m_codepoint = codepoint.value;
        }
    }
}

void FontRenderTest::onMacro(const Event::Macro& macro) {

}

void FontRenderTest::onWindowResize(const Event::WindowResize& window_resize) {

}

void FontRenderTest::draw(Draw::CallQueue& draw_buffer) {

    /*std::array<FontVertex, 6> vertices;

    glm::vec2 draw_pos = calcDrawPos();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x, draw_pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(m_size.x * m_font->calcPXScaler(24), m_size.y * m_font->calcPXScaler(24), 0.0f));

    // top right
    vertices[0].pos = { 0.5f, 0.5f }; // top right
    vertices[0].tex_coords = { 1.0f, 1.0f, m_codepoint - m_font->getStartCodepoint() };
    vertices[0].color = {0.0f, 0.0f, 0.0f, 0.0f};
    // bottom right
    vertices[1].pos = {0.5f, -0.5f}; // top right
    vertices[1].tex_coords = {1.0f, 0.0f, m_codepoint - m_font->getStartCodepoint() };
    vertices[1].color = {0.0f, 0.0f, 0.0f, 0.0f};
    // top left
    vertices[2].pos = {-0.5f, 0.5f}; // top right
    vertices[2].tex_coords = {0.0f, 1.0f, m_codepoint - m_font->getStartCodepoint() };
    vertices[2].color = {0.0f, 0.0f, 0.0f, 0.0f};
    // bottom right
    vertices[3].pos = {0.5f, -0.5f}; // top right
    vertices[3].tex_coords = {1.0f, 0.0f, m_codepoint - m_font->getStartCodepoint() };
    vertices[3].color = {0.0f, 0.0f, 0.0f, 0.0f};
    // bottom left
    vertices[4].pos = {-0.5f, -0.5f}; // top right
    vertices[4].tex_coords = {0.0f, 0.0f, m_codepoint - m_font->getStartCodepoint() };
    vertices[4].color = {0.0f, 0.0f, 0.0f, 0.0f};
    // top left
    vertices[5].pos = { -0.5f, 0.5f}; // top right
    vertices[5].tex_coords = {0.0f, 1.0f, m_codepoint - m_font->getStartCodepoint() };
    vertices[5].color = {0.0f, 0.0f, 0.0f, 0.0f};

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
    glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW); CheckGLError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);  CheckGLError();

    Call new_draw;
    new_draw.handles = m_vao;
    new_draw.shader = m_shader;
    new_draw.count = 6;
    new_draw.uniforms[0].setValue(model);
    new_draw.uniforms[1].setValue(Uniform::ArrayTexture(m_font->getHandle().value(), 0));
    new_draw.uniforms[2].setValue(m_font->getMSDFPixelRange());
    draw_buffer.push_back(new_draw);

    debugDraw(draw_buffer);*/
}