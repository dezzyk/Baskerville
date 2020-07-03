//
// Created by Feed on 5/30/2020.
//

#include "widget.h"
#include "shader.h"

#include <array>

Widget::Widget(Widget* parent) : m_parent(parent) {}

Widget::~Widget() {}

Widget::Widget(Widget&& other) noexcept :
        size(other.size),
        offset(other.offset),
        m_parent(other.m_parent),
        anchor(other.anchor),
        m_debug_draw(std::move(other.m_debug_draw)) {}

b32 Widget::pointIntersect(glm::vec2 pos) {
    glm::vec2 draw_size = calcDrawSize();
    glm::vec2 draw_pos = calcDrawPos();
    return pos.x >= (draw_pos.x - (draw_size.x / 2)) && pos.x <= (draw_pos.x + (draw_size.x / 2)) &&
           pos.y >= (draw_pos.y - (draw_size.y / 2)) && pos.y <= (draw_pos.y + (draw_size.y / 2));
}

const glm::vec2& Widget::getSize() const {
    return size;
}

const Widget* Widget::getParent() const {
    return m_parent;
}

void Widget::debugViewUpdate() {
    if(!m_debug_draw.draw_attempted) {
        if (m_debug_draw.shader == nullptr) {

            m_debug_draw.shader = Shader::Cache::fetch("box_draw");
            if(m_debug_draw.shader == nullptr) {
                std::string vert =
#include "shader/box_draw.vert"
                ;
                std::string frag =
#include "shader/box_draw.frag"
                ;
                m_debug_draw.shader = Shader::Cache::load("box_draw", vert, frag);
            }

        }
        m_debug_draw.draw_attempted = true;
    }
    if((m_debug_draw.shader != nullptr && m_debug_draw.shader->getHandle().has_value()) && m_debug_draw.context.valid()) {

        static std::array<Draw::Box, 8> boxes;
        boxes = {};

        glm::vec2 draw_size = calcDrawSize();
        glm::vec2 draw_pos = calcDrawPos();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 2.0f, draw_pos.y + (draw_size.y / 2) - 8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[0] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 8.0f, draw_pos.y + (draw_size.y / 2) - 2, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[1] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 2.0f, draw_pos.y - (draw_size.y / 2) + 8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[2] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 8.0f, draw_pos.y - (draw_size.y / 2) + 2.0, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[3] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 2, draw_pos.y - (draw_size.y / 2) + 8.0, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[4] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 8, draw_pos.y - (draw_size.y / 2) + 2.0, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[5] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 2, draw_pos.y + (draw_size.y / 2) - 8, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
        boxes[6] *= model;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 8, draw_pos.y + (draw_size.y / 2) - 2, 0.0f));
        model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
        boxes[7] *= model;

        for(auto& box : boxes) {
            for(auto& vert : box.vertices) {
                vert.color = {1.0f, 0.0f, 0.0f, 0.5f};
            }
        }

        m_debug_draw.context.boxUpload<8>(boxes);

    }
}

void Widget::debugViewDraw(Draw::CallQueue& draw_buffer) {
    if(!m_debug_draw.draw_attempted) {
        if (m_debug_draw.shader == nullptr) {

            m_debug_draw.shader = Shader::Cache::fetch("box_draw");
            if(m_debug_draw.shader == nullptr) {
                std::string vert =
                #include "shader/box_draw.vert"
                        ;
                std::string frag =
                #include "shader/box_draw.frag"
                        ;
                m_debug_draw.shader = Shader::Cache::load("box_draw", vert, frag);
            }

        }
        m_debug_draw.draw_attempted = true;
    }
    if((m_debug_draw.shader != nullptr && m_debug_draw.shader->getHandle().has_value()) && m_debug_draw.context.valid()) {

        Draw::Call draw;
        draw.context = &m_debug_draw.context;
        draw.shader = m_debug_draw.shader;
        draw.count = 8; // Magic number will come back to haunt me I just know it
        draw_buffer.push_back(draw);

    }
}

glm::vec2 Widget::calcDrawPos() {

    // If there isnt a parent, the widget is treated as a root. Its anchor is hardset to 0,0 and its offset is ignored.
    // Best that the widget being treated as the root set its size to the value of a resize even.

    glm::vec2 draw_size = calcDrawSize();
    glm::vec2 draw_pos = {draw_size.x / 2, draw_size.y / 2};
    if(m_parent != nullptr) {
        glm::vec2 parent_draw_size = m_parent->calcDrawSize();
        draw_pos = m_parent->calcDrawPos();
        switch (anchor) {
            case Widget::Anchor::TopLeft  : {
                draw_pos.x += ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y -= ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
            case Widget::Anchor::Top  : {
                draw_pos.x += (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y -= ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
            case Widget::Anchor::TopRight  : {
                draw_pos.x -= ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y -= ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
            case Widget::Anchor::Left  : {
                draw_pos.x += ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y += (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
            case Widget::Anchor::Center  : {
                draw_pos.x += (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y += (offset.y * (parent_draw_size.y - draw_size.y));
            }
                break;
            case Widget::Anchor::Right  : {
                draw_pos.x -= ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y += (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
            case Widget::Anchor::BottomLeft  : {
                draw_pos.x += ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y += ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
            case Widget::Anchor::Bottom  : {
                draw_pos.x += (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y += ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
            case Widget::Anchor::BottomRight  : {
                draw_pos.x -= ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (offset.x * (parent_draw_size.x - draw_size.x));
                draw_pos.y += ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (offset.y * (parent_draw_size.y - draw_size.y));
            }
            break;
        }
    }
    return draw_pos;
}

glm::vec2 Widget::calcDrawSize() {
    if(m_parent != nullptr) {
        glm::vec2 final_size = size;
        f32 scale = Platform::getViewportScaler();
        if(!unscaled_width) { final_size.x *= scale; }
        if(!unscaled_height) { final_size.x *= scale; }
        return final_size;
    }
    return size;
}