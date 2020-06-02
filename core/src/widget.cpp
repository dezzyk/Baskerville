//
// Created by Feed on 5/30/2020.
//

#include "widget.h"
#include "shader.h"

#include "glm/gtc/matrix_transform.hpp"

#include <array>

Widget::Widget(Widget& widget) : m_parent(&widget) {}

Widget::~Widget() {
    if(m_debug_draw_params.vao.has_value()){
        glDeleteVertexArrays(1, &m_debug_draw_params.vao.value()); CheckGLError();
        m_debug_draw_params.vao = std::nullopt;
    }
    if(m_debug_draw_params.vbo.has_value()){
        glDeleteBuffers(1, &m_debug_draw_params.vbo.value()); CheckGLError();
        m_debug_draw_params.vbo = std::nullopt;
    }
}

Shader* shader = nullptr;
std::optional<u32> vao;
std::optional<u32> vbo;
b32 draw_attempted = false;

void Widget::debugDraw(Window::DrawBuffer& draw_buffer) {
    if(!m_debug_draw_params.draw_attempted) {
        if (m_debug_draw_params.shader == nullptr) {

            m_debug_draw_params.shader = Shader::Cache::fetch("debug_draw");
            if(m_debug_draw_params.shader == nullptr) {
                std::string vert =
                #include "shader/debug_draw.vert"
                        ;
                std::string frag =
                #include "shader/debug_draw.frag"
                        ;
                m_debug_draw_params.shader = Shader::Cache::load("debug_draw", vert, frag);
            }

        }
        if (!m_debug_draw_params.vbo.has_value()) {
            u32 vbo = 0;
            glGenBuffers(1, &vbo);  CheckGLError();
            if (vbo != 0) {
                m_debug_draw_params.vbo = vbo;
            } else {
                std::cout << "Failed to create vbo | widget default" << std::endl;
            }
        }
        if (!m_debug_draw_params.vao.has_value() && m_debug_draw_params.vbo.has_value()) {
            u32 vao = 0;
            glGenVertexArrays(1, &vao); CheckGLError();
            if (vao != 0) {
                glBindVertexArray(vao); CheckGLError();
                glBindBuffer(GL_ARRAY_BUFFER, m_debug_draw_params.vbo.value()); CheckGLError();
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0); CheckGLError();
                glEnableVertexAttribArray(0); CheckGLError();
                glBindVertexArray(0); CheckGLError();
                m_debug_draw_params.vao = vao;
            } else {
                std::cout << "Failed to create vao | widget default" << std::endl;
            }
        }
        m_debug_draw_params.draw_attempted = true;
    }
    if((m_debug_draw_params.shader != nullptr && m_debug_draw_params.shader->getHandle().has_value()) && m_debug_draw_params.vao.has_value() && m_debug_draw_params.vbo.has_value()) {

        std::array<glm::vec2, 48> vertices;

        glm::vec2 draw_pos = calcDrawPos();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + 2.0f, draw_pos.y + m_size.y - 8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        // First Tri
        vertices[0] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[1] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[2] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[3] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[4] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[5] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + 8.0f,draw_pos.y +  m_size.y-2, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        // First Tri
        vertices[6] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[7] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[8] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[9] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[10] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[11] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + 2.0f,draw_pos.y +  8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        // First Tri
        vertices[12] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[13] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[14] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[15] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[16] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[17] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + 8.0f,draw_pos.y +  2.0, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        // First Tri
        vertices[18] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[19] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[20] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[21] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[22] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[23] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + m_size.x - 2,draw_pos.y +  8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        // First Tri
        vertices[24] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[25] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[26] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[27] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[28] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[29] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + m_size.x-8,draw_pos.y +  2.0, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        // First Tri
        vertices[30] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[31] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[32] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[33] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[34] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[35] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + m_size.x-2,draw_pos.y +  m_size.y-8, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        // First Tri
        vertices[36] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[37] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[38] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[39] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[40] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[41] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + m_size.x-8,draw_pos.y +  m_size.y-2, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        // First Tri
        vertices[42] = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // top right
        vertices[43] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[44] = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // top left
        // Second Tri
        vertices[45] = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // bottom right
        vertices[46] = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // bottom left
        vertices[47] = model * glm::vec4(-1.0f, 1.0f, 0.0, 1.0f); // top left


        glBindBuffer(GL_ARRAY_BUFFER, m_debug_draw_params.vbo.value());  CheckGLError();
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW); CheckGLError();
        glBindBuffer(GL_ARRAY_BUFFER, 0);  CheckGLError();

        Draw draw;
        draw.vao = m_debug_draw_params.vao.value();
        draw.shader = m_debug_draw_params.shader;
        draw.count = vertices.size();
        draw_buffer.push_back(draw);

    }
}

glm::vec2 Widget::calcDrawPos() {

    // If there isnt a parent, the widget is treated as a root. Its anchor is hardset to 0,0 and its offset is ignored.
    // Best that the widget being treated as the root set its size to the value of a resize even.

    glm::vec2 draw_pos = {0.0f, 0.0f};
    if(m_parent != nullptr) {
        draw_pos = m_parent->calcDrawPos();
        switch (m_anchor) {
            case Widget::Anchor::TopLeft  : {
                draw_pos.x += (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_parent->m_size.y - m_size.y) - (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Top  : {
                draw_pos.x += ((m_parent->m_size.x / 2) - (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_parent->m_size.y - m_size.y) - (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::TopRight  : {
                draw_pos.x += (m_parent->m_size.x - m_size.x) - (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_parent->m_size.y - m_size.y) - (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Left  : {
                draw_pos.x += (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += ((m_parent->m_size.y / 2) - (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Center  : {
                draw_pos.x += ((m_parent->m_size.x / 2) - (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += ((m_parent->m_size.y / 2) - (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
                break;
            case Widget::Anchor::Right  : {
                draw_pos.x += (m_parent->m_size.x - m_size.x) - (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += ((m_parent->m_size.y / 2) - (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::BottomLeft  : {
                draw_pos.x += (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Bottom  : {
                draw_pos.x += ((m_parent->m_size.x / 2) - (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::BottomRight  : {
                draw_pos.x += (m_parent->m_size.x - m_size.x) - (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
        }
    }
    return draw_pos;
}