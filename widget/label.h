//
// Created by Feed on 6/14/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "font.h"
#include "draw.h"

#include <string>

class Label : public Widget {
public:
    Label() = delete;
    explicit Label(Widget* parent);
    // string value is not stored internally to prevent conflicts.
    void setValue(const std::string& value, const Font* font, u32 pixel_height, glm::vec4 color);
    void draw(Draw::CallQueue& draw_buffer, f32 scale) override ;
    void setWidth(u32 width);
private:
    //u32 m_pixel_width = 0;
    u32 m_pixel_height = 0;
    Draw::Context m_draw_context;
    const Shader* m_shader = nullptr;
    const Font* m_font = nullptr;
};
