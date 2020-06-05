//
// Created by Feed on 5/30/2020.
//

#include "widget.h"
#include "shader.h"

#include "glm/gtc/matrix_transform.hpp"

#include <array>

Widget::Widget(Widget& widget) : m_parent(&widget) {}

Widget::~Widget() {
    if(m_debug_draw.draw_handles.vao.has_value()){
        glDeleteVertexArrays(1, &m_debug_draw.draw_handles.vao.value()); CheckGLError();
        m_debug_draw.draw_handles.vao = std::nullopt;
    }
    if(m_debug_draw.draw_handles.vbo.has_value()){
        glDeleteBuffers(1, &m_debug_draw.draw_handles.vbo.value()); CheckGLError();
        m_debug_draw.draw_handles.vbo = std::nullopt;
    }
}

Shader* shader = nullptr;
std::optional<u32> vao;
std::optional<u32> vbo;
b32 draw_attempted = false;

glm::vec2 Widget::getSize() const{
    return m_size;
}

glm::vec2 Widget::getOffset() const{
    return m_offset;
}

Widget::Anchor Widget::getAnchor() const{
    return m_anchor;
}

void Widget::debugDraw(Draw::CallQueue& draw_buffer) {
    if(!m_debug_draw.draw_attempted) {
        if (m_debug_draw.shader == nullptr) {

            m_debug_draw.shader = Shader::Cache::fetch("debug_draw");
            if(m_debug_draw.shader == nullptr) {
                std::string vert =
                #include "shader/debug_draw.vert"
                        ;
                std::string frag =
                #include "shader/debug_draw.frag"
                        ;
                m_debug_draw.shader = Shader::Cache::load("debug_draw", vert, frag);
            }

        }
        if (!m_debug_draw.draw_handles.vbo.has_value()) {
            generateDrawHandles(m_debug_draw.draw_handles);
            if (!m_debug_draw.draw_handles.vbo.has_value()) {
                std::cout << "Failed to create debug draw handles | widget default" << std::endl;
            } else {
                glBindVertexArray(m_debug_draw.draw_handles.vao.value()); CheckGLError();
                glBindBuffer(GL_ARRAY_BUFFER, m_debug_draw.draw_handles.vbo.value()); CheckGLError();
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0); CheckGLError();
                glEnableVertexAttribArray(0); CheckGLError();
                glBindVertexArray(0); CheckGLError();
            }
        }
        m_debug_draw.draw_attempted = true;
    }
    if((m_debug_draw.shader != nullptr && m_debug_draw.shader->getHandle().has_value()) && m_debug_draw.draw_handles.vao.has_value() && m_debug_draw.draw_handles.vbo.has_value()) {

        std::array<Draw::Box, 8> boxes;

        glm::vec2 draw_pos = calcDrawPos();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (m_size.x / 2) + 2.0f, draw_pos.y + (m_size.y / 2) - 8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[0] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (m_size.x / 2) + 8.0f,draw_pos.y + (m_size.y / 2) - 2, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[1] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (m_size.x / 2)+ 2.0f,draw_pos.y - (m_size.y / 2) + 8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[2] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (m_size.x / 2) + 8.0f,draw_pos.y - (m_size.y / 2)+  2.0, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[3] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + (m_size.x / 2) - 2,draw_pos.y - (m_size.y / 2)+  8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[4] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x +(m_size.x / 2)-8,draw_pos.y - (m_size.y / 2)+  2.0, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[5] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + (m_size.x / 2)-2,draw_pos.y +  (m_size.y / 2)-8, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[6] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + (m_size.x / 2)-8,draw_pos.y +  (m_size.y / 2)-2, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[7] *= model;

        glBindBuffer(GL_ARRAY_BUFFER, m_debug_draw.draw_handles.vbo.value());  CheckGLError();
        glBufferData(GL_ARRAY_BUFFER, sizeof(Draw::Box) * boxes.size(), boxes.data(), GL_DYNAMIC_DRAW); CheckGLError();
        glBindBuffer(GL_ARRAY_BUFFER, 0);  CheckGLError();

        Draw::Call draw;
        draw.handles = m_debug_draw.draw_handles;
        draw.shader = m_debug_draw.shader;
        draw.count = boxes.size() * boxes[0].vertices.size();
        draw_buffer.push_back(draw);

    }
}

void Widget::generateDrawHandles(Draw::Handles& draw_handles) {
    if(draw_handles.vbo.has_value()) {
        glDeleteBuffers(1, &draw_handles.vbo.value());
    }
    if(draw_handles.vao.has_value()) {
        glDeleteVertexArrays(1, &draw_handles.vao.value());
    }
    u32 vbo_handle, vao_handle;
    glGenBuffers(1, &vbo_handle); CheckGLError();
    glGenVertexArrays(1, &vao_handle); CheckGLError();
    if(vbo_handle != 0) {
        if(vao_handle != 0) {
            draw_handles.vbo = vbo_handle;
            draw_handles.vao = vao_handle;
            glBindVertexArray(draw_handles.vao.value()); CheckGLError();
            glBindBuffer(GL_ARRAY_BUFFER, draw_handles.vbo.value()); CheckGLError();
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); CheckGLError();
            glEnableVertexAttribArray(0); CheckGLError();
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(4* sizeof(float))); CheckGLError();
            glEnableVertexAttribArray(1); CheckGLError();
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7* sizeof(float))); CheckGLError();
            glEnableVertexAttribArray(2); CheckGLError();
            glBindVertexArray(0); CheckGLError();
        } else {
            glDeleteBuffers(1, &vbo_handle); CheckGLError();
        }
    }
}

glm::vec2 Widget::calcDrawPos() {

    // If there isnt a parent, the widget is treated as a root. Its anchor is hardset to 0,0 and its offset is ignored.
    // Best that the widget being treated as the root set its size to the value of a resize even.

    glm::vec2 draw_pos = {m_size.x/2, m_size.y/2};
    if(m_parent != nullptr) {
        draw_pos = m_parent->calcDrawPos();
        switch (m_anchor) {
            case Widget::Anchor::TopLeft  : {
                draw_pos.x += ((m_parent->m_size.x / -2) + (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y -= ((m_parent->m_size.y / -2) + (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Top  : {
                draw_pos.x += (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y -= ((m_parent->m_size.y / -2) + (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::TopRight  : {
                draw_pos.x -= ((m_parent->m_size.x / -2) + (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y -= ((m_parent->m_size.y / -2) + (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Left  : {
                draw_pos.x += ((m_parent->m_size.x / -2) + (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Center  : {
                draw_pos.x += (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
                break;
            case Widget::Anchor::Right  : {
                draw_pos.x -= ((m_parent->m_size.x / -2) + (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::BottomLeft  : {
                draw_pos.x += ((m_parent->m_size.x / -2) + (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += ((m_parent->m_size.y / -2) + (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::Bottom  : {
                draw_pos.x += (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += ((m_parent->m_size.y / -2) + (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
            case Widget::Anchor::BottomRight  : {
                draw_pos.x -= ((m_parent->m_size.x / -2) + (m_size.x / 2)) + (m_offset.x * (m_parent->m_size.x - m_size.x));
                draw_pos.y += ((m_parent->m_size.y / -2) + (m_size.y / 2)) + (m_offset.y * (m_parent->m_size.y - m_size.y));
            }
            break;
        }
    }
    return draw_pos;
}