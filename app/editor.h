//
// Created by Feed on 6/19/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "label.h"
#include "font.h"
#include "project.h"
#include "cache.h"

#include "nlohmann/json.hpp"

#include <string>
#include <vector>

using json = nlohmann::json ;

class Editor : public Widget {
public:
    struct Line {
        Label label;
        std::string value;
        Line* prev = nullptr;
    };
    explicit Editor(Widget* parent, CacheBank& cache);
    Draw::RedrawFlag update(f64 delta) override ;
    //Draw::RedrawFlag onCodepoint(const Event::Codepoint& codepoint) override ;
    Draw::RedrawFlag onTextInput(const Event::TextInput& text) override ;
    Draw::RedrawFlag onMacro(const Event::Macro& macro) override ;
    Draw::RedrawFlag onMouseClick(Event::MouseClick mouse_click) override ;
    void draw(Draw::CallQueue& draw_buffer, f32 scale) override ;
private:
    Project m_project;
    const Font* m_font = nullptr;
    std::array<Line, 3> m_lines;
    Line* m_prev_line = nullptr;
    Line* m_active_line = nullptr;
    Line* m_next_line = nullptr;
    u32 m_font_pixel_height = 24;
    f32 m_line_lerp_scaler = 1.0f;
    f64 m_lint_lerp_time = 1.0;
};
