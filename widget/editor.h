//
// Created by Feed on 6/4/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "chapter_number.h"

class Editor : public Widget {
public:
    Editor();
    Editor(Widget* parent);
    void onCodepoint(const Event::Codepoint& codepoint) override ;
    void onMacro(const Event::Macro& macro) override ;
    void onWindowResize(const Event::WindowResize& window_resize) override ;
    void draw(Draw::CallQueue& draw_buffer) override ;
private:
    ChapterNumber m_chaper_number;
};
