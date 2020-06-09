//
// Created by Feed on 6/4/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "single_char.h"

class Editor : public Widget {
public:
    Editor();
    explicit Editor(Widget* parent);
    void update() override ;
    void onCodepoint(const Event::Codepoint& codepoint) override ;
    void onMacro(const Event::Macro& macro) override ;
    void onMouseClick(Event::MouseClick mouse_click) override ;
    void onWindowResize() override ;
    void draw(Draw::CallQueue& draw_buffer) override ;
private:
    SingleChar m_chaper_number;
};
