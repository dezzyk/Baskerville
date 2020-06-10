//
// Created by Feed on 6/9/2020.
//

#pragma once

#include "common.h"
#include "widget.h"

class Label : public Widget {
public:
    Label();
    explicit Label(Widget* parent);
    void update() override ;
    void onCodepoint(const Event::Codepoint& codepoint) override ;
    void onMacro(const Event::Macro& macro) override ;
    void onMouseClick(Event::MouseClick mouse_click) override ;
    void onWindowResize() override ;
    void draw(Draw::CallQueue& draw_buffer) override ;
private:

};