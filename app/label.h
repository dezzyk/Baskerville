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
    void setValue(const std::string& value, const Font* font, u32 pixel_height);
    void setColor(glm::vec3 color);
    void setAlpha(f32 alpha);
    void setWidth(u32 width);
    glm::vec2& getOffsetRef();
private:
    Draw::RedrawFlag derivedUpdate(f64 delta) override { return false; };
    void derivedDraw(Draw::CallQueue& draw_buffer, f32 scale) override ;

    Draw::Context m_draw_context;
    glm::vec4 m_color = {0.0f, 0.0f, 0.0f, 1.0f};
    const Shader* m_shader = nullptr;
    const Font* m_font = nullptr;
    u32 m_pixel_height = 0;

};
