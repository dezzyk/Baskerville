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
        f32 fade_offset = -1.0;
    };
    explicit Editor(Widget* parent, CacheBank& cache);
    void update() override ;
    void onCodepoint(const Event::Codepoint& codepoint) override ;
    void onTextInput(const Event::TextInput& text) override ;
    void onMacro(const Event::Macro& macro) override ;
    void onMouseClick(Event::MouseClick mouse_click) override ;
    void draw(Draw::CallQueue& draw_buffer, f32 scale) override ;
private:
    Project m_project;
    const Font* m_font = nullptr;
    std::vector<Line> m_lines;
    Line* cur_line = nullptr;
    Line* prev_line = nullptr;
    u32 m_cur_line_index = 0;
    u32 m_font_pixel_height = 24;
};
