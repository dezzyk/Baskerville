//
// Created by Feed on 6/19/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "label.h"

#include <string>
#include <vector>

class Editor : public Widget {
public:
    struct Line {
        Label label;
        std::string value;
    };
    explicit Editor(Widget* parent);
    void update() override ;
    void onCodepoint(const Event::Codepoint& codepoint) override ;
    void onMacro(const Event::Macro& macro) override ;
    void onMouseClick(Event::MouseClick mouse_click) override ;
    void draw(Draw::CallQueue& draw_buffer, f32 scale) override ;
private:
    std::vector<Line> m_lines;
    u32 m_active_line_index = 0;
};
