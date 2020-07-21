//
// Created by Feed on 7/7/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "event.h"
#include "draw.h"

class Menu : public Widget, public Event::Handler {
public:
    explicit Menu(Widget* parent);
    Draw::RedrawFlag onTextInput(const Event::TextInput& text) override ;
    Draw::RedrawFlag onMacro(const Event::Macro& macro) override ;
    Draw::RedrawFlag onMouseClick(Event::MouseClick mouse_click) override ;

private:
    Draw::RedrawFlag derivedUpdate(f64 delta) override ;
    void derivedDraw(Draw::Queue& queue) override ;

    Draw::Context m_draw_context;

};
