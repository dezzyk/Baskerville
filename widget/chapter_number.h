//
// Created by Feed on 6/4/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "font.h"
#include "shader.h"

class ChapterNumber : public Widget {
public:
    ChapterNumber();
    ChapterNumber(Widget &);
    void onCodepoint(const Event::Codepoint& codepoint);
    void onMacro(const Event::Macro& macro);
    void onWindowResize(const Event::WindowResize& resize);
    void draw(Draw::CallQueue& draw_buffer);
private:
    const Font* m_font = nullptr;
    const Shader* m_shader = nullptr;
    Draw::Handles m_draw_handles;
};
