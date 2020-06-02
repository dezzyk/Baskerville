//
// Created by Feed on 5/31/2020.
//

#include "font_render_test.h"

#include "glm/gtc/matrix_transform.hpp"

FontRenderTest::FontRenderTest(Widget& parent) : Widget(parent) {
    m_anchor = Widget::Anchor::Center;
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
        u32 vbo, vao;
        glGenBuffers(1, &vbo); CheckGLError();
        glGenVertexArrays(1, &vao); CheckGLError();
        if(vbo != 0) {
            if(vao != 0) {
                m_vbo = vbo;
                m_vao = vao;
            } else {
                glDeleteBuffers(1, &vbo); CheckGLError();
            }
        }
        if(m_vbo.has_value() && m_vao.has_value()) {
            glBindVertexArray(m_vao.value()); CheckGLError();
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value()); CheckGLError();
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); CheckGLError();
            glEnableVertexAttribArray(0); CheckGLError();
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(2* sizeof(float))); CheckGLError();
            glEnableVertexAttribArray(1); CheckGLError();
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5* sizeof(float))); CheckGLError();
            glEnableVertexAttribArray(2); CheckGLError();
            glBindVertexArray(0); CheckGLError();
        }
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

void FontRenderTest::draw(Window::DrawBuffer& draw_buffer) {

    struct FontVertex {
        glm::vec2 pos;
        glm::vec3 tex_coords;
        glm::vec4 color;
    };

    std::array<FontVertex, 6> vertices;

    glm::vec2 draw_pos = calcDrawPos();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x + (m_size.x / 2),draw_pos.y + (m_size.y / 2), 0.0f));
    model = glm::scale(model, glm::vec3(m_size.x, m_size.y, 0.0f));

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

    Draw new_draw;
    new_draw.vao = m_vao;
    new_draw.shader = m_shader;
    new_draw.count = 6;
    new_draw.uniforms[0].setValue(model);
    new_draw.uniforms[1].setValue(Uniform::ArrayTexture(m_font->getHandle().value(), 0));
    new_draw.uniforms[2].setValue(m_font->getMSDFPixelRange());
    draw_buffer.push_back(new_draw);

    debugDraw(draw_buffer);
}