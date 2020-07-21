//
// Created by Feed on 7/20/2020.
//

#include "widget.h"
#include "platform.h"

std::optional<glm::vec2> Widget::calcDrawPos(entt::entity entity, entt::registry &reg) {

    if(reg.has<Widget>(entity) && reg.valid(entity)) {
        Widget& widget = reg.get<Widget>(entity);
        glm::vec2 draw_size, draw_pos, parent_draw_size;
        draw_size = calcDrawSize(entity, reg).value();
        f32 scale = Platform::getViewportScaler();
        // If a parent doesnt exist, use the viewport params as the parent params
        if(widget.parent.has_value()) {
            parent_draw_size = calcDrawSize(widget.parent.value(), reg).value();
            draw_pos = calcDrawPos(widget.parent.value(), reg).value();
        } else {
            parent_draw_size = Platform::getViewportSize();
            draw_pos = {parent_draw_size.x / 2, parent_draw_size.y / 2};
        }
        switch (widget.alignment) {
            case Widget::Align::TopLeft  : {
                draw_pos.x += ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (widget.offset.x * scale);
                draw_pos.y -= ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (widget.offset.y * scale);
            }
                break;
            case Widget::Align::Top  : {
                draw_pos.x += (widget.offset.x * scale);
                draw_pos.y -= ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (widget.offset.y * scale);
            }
                break;
            case Widget::Align::TopRight  : {
                draw_pos.x -= ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (widget.offset.x * scale);
                draw_pos.y -= ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (widget.offset.y * scale);
            }
                break;
            case Widget::Align::Left  : {
                draw_pos.x += ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (widget.offset.x * scale);
                draw_pos.y += (widget.offset.y * scale);
            }
                break;
            case Widget::Align::Center  : {
                draw_pos.x += (widget.offset.x * scale);
                draw_pos.y += (widget.offset.y * scale);
            }
                break;
            case Widget::Align::Right  : {
                draw_pos.x -= ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (widget.offset.x * scale);
                draw_pos.y += (widget.offset.y * scale);
            }
                break;
            case Widget::Align::BottomLeft  : {
                draw_pos.x += ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (widget.offset.x * scale);
                draw_pos.y += ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (widget.offset.y * scale);
            }
                break;
            case Widget::Align::Bottom  : {
                draw_pos.x += (widget.offset.x * scale);
                draw_pos.y += ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (widget.offset.y * scale);
            }
                break;
            case Widget::Align::BottomRight  : {
                draw_pos.x -= ((parent_draw_size.x / -2) + (draw_size.x / 2)) + (widget.offset.x * scale);
                draw_pos.y += ((parent_draw_size.y / -2) + (draw_size.y / 2)) + (widget.offset.y * scale);
            }
                break;
        }
        return draw_pos;
    }
    return std::nullopt;

}

std::optional<glm::vec2> Widget::calcDrawSize(entt::entity entity, entt::registry &reg) {

    if(reg.has<Widget>(entity) && reg.valid(entity)) {
        Widget& widget = reg.get<Widget>(entity);
        glm::vec2 final_size = widget.size;
        f32 scale = Platform::getViewportScaler();
        if(!widget.unscaled_width) { final_size.x *= scale; }
        if(!widget.unscaled_height) { final_size.y *= scale; }
        return final_size;
    }
    return std::nullopt;

}