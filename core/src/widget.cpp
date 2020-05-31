//
// Created by Feed on 5/30/2020.
//

#include "widget.h"
#include "shader.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <array>

namespace {
    const Shader* default_draw_shader = nullptr;
    std::optional<u32> default_draw_vao;
    std::optional<u32> default_draw_vbo;
    b32 default_draw_attempted = false;
    u32 widget_count = 0;
}

Widget::Widget() {
    widget_count++;
}

Widget::Widget(Widget& widget) : m_parent(&widget) {
    widget_count++;
}

Widget::~Widget() {
    widget_count--;
    if(widget_count == 0) {
        default_draw_shader = nullptr;
        if(default_draw_vao.has_value()){
            glDeleteVertexArrays(1, &default_draw_vao.value());
            default_draw_vao = std::nullopt;
        }
        if(default_draw_vbo.has_value()){
            glDeleteBuffers(1, &default_draw_vbo.value());
            default_draw_vbo = std::nullopt;
        }
    }
}

void Widget::debugDraw(const Window::Viewport &viewport) {
    if(!default_draw_attempted) {
        if (default_draw_shader == nullptr) {

            std::string vert = "#version 430 core\n"
                                   "layout (location = 0) in vec2 aPos;\n"
                                   "layout (location = 0) uniform mat4 proj;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position = proj * vec4(aPos, 0.0, 1.0);\n"
                                   "}";

            std::string frag = "#version 430 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                                   "}";

            default_draw_shader = Shader::Cache::load("widget_debug_draw", vert, frag);

        }
        if (!default_draw_vbo.has_value()) {
            u32 vbo = 0;
            glGenBuffers(1, &vbo);
            CheckGLError();
            if (vbo != 0) {
                default_draw_vbo = vbo;
            } else {
                std::cout << "Failed to create vbo | widget default" << std::endl;
            }
        }
        if (!default_draw_vao.has_value() && default_draw_vbo.has_value()) {
            u32 vao = 0;
            glGenVertexArrays(1, &vao);
            CheckGLError();
            if (vao != 0) {
                glBindVertexArray(vao);
                CheckGLError();
                glBindBuffer(GL_ARRAY_BUFFER, default_draw_vbo.value());
                CheckGLError();
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
                CheckGLError();
                glEnableVertexAttribArray(0);
                CheckGLError();
                glBindVertexArray(0);
                CheckGLError();
                default_draw_vao = vao;
            } else {
                std::cout << "Failed to create vao | widget default" << std::endl;
            }
        }
        default_draw_attempted = true;
    }
    if((default_draw_shader!= nullptr && default_draw_shader->getHandle().has_value()) && default_draw_vao.has_value() && default_draw_vbo.has_value()) {

        std::array<glm::vec2, 48> vertices;

        glm::vec2 draw_pos = calcDrawPos(viewport);

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

        glm::mat4 proj = glm::ortho(0.0f, viewport.x, 0.0f, viewport.y);
        glUseProgram(default_draw_shader->getHandle().value()); CheckGLError();
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(proj));
        glBindVertexArray(default_draw_vao.value()); CheckGLError();
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW); CheckGLError();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()); CheckGLError();
        glBindVertexArray(0);
    }
}

glm::vec2 Widget::calcDrawPos(const Window::Viewport &viewport) {

    //If there isnt a parent, the widget is treated as the root and uses the viewport as a psuedo parent to build its draw_pos.

    glm::vec2 draw_pos;
    if(m_parent != nullptr) {
        draw_pos = m_parent->calcDrawPos(viewport);
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
    } else {
        switch (m_anchor) {
            case Widget::Anchor::TopLeft  : {
                draw_pos = {0.0f, viewport.y - m_size.y};
            }
            break;
            case Widget::Anchor::Top  : {
                draw_pos = {(viewport.x / 2) - (m_size.x / 2), viewport.y - m_size.y};
            }
            break;
            case Widget::Anchor::TopRight  : {
                draw_pos = {draw_pos.x - m_size.x, viewport.y - m_size.y};
            }
            break;
            case Widget::Anchor::Left  : {
                draw_pos = {0.0f, (viewport.y / 2) - (m_size.y / 2)};
            }
            break;
            case Widget::Anchor::Center  : {
                draw_pos = {(viewport.x / 2) - (m_size.x / 2), (viewport.y / 2) - (m_size.y / 2)};
            }
            break;
            case Widget::Anchor::Right  : {
                draw_pos = {viewport.x - m_size.x, (viewport.y / 2) - (m_size.y / 2)};
            }
            break;
            case Widget::Anchor::BottomLeft  : {
                draw_pos = {0.0f, 0.0f};
            }
            break;
            case Widget::Anchor::Bottom  : {
                draw_pos = {(viewport.x / 2) - (m_size.x / 2), 0.0};
            }
            break;
            case Widget::Anchor::BottomRight  : {
                draw_pos = {viewport.x - m_size.x, 0.0f};
            }
            break;
        }
    }
    return draw_pos;
}