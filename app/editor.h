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

class Editor : public Widget, public Event::Handler {
public:
    struct Line {
        Label label;
        i32 cur_id = -1;
    };
    explicit Editor(Widget* parent, CacheBank& cache);
    Draw::RedrawFlag onTextInput(const Event::TextInput& text) override ;
    Draw::RedrawFlag onMacro(const Event::Macro& macro) override ;
    Draw::RedrawFlag onMouseClick(Event::MouseClick mouse_click) override ;
private:
    Draw::RedrawFlag derivedUpdate(f64 delta) override ;
    void derivedDraw(Draw::Queue& queue) override ;

    Project m_project;
    const Font* m_font = nullptr;
    std::vector<Line> m_lines;
    //Line* m_prev_line_4 = nullptr;
    //Line* m_prev_line_3 = nullptr;
    //Line* m_prev_line_2 = nullptr;
    //Line* m_prev_line_1 = nullptr;
    //Line* m_active_line = nullptr;
    //Line* m_next_line = nullptr;
    i32 m_selected_line_index = -1;
    f32 m_font_pixel_height = 24.0f;
    f32 m_line_lerp_scaler = 1.0f;
    f64 m_line_lerp_time = 0.5;
    f64 m_line_lerp_accumulator = 0.0;
    f32 m_line_base_offset = -100.0f;
};
