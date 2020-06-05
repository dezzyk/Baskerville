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
    Editor(Widget& parent);
    void onCodepoint(const Event::Codepoint& codepoint);
    void onMacro(const Event::Macro& macro);
    void onWindowResize(const Event::WindowResize& window_resize);
    void draw(Draw::CallQueue& draw_buffer);
private:
    ChapterNumber m_chaper_number;
};
