//
// Created by Feed on 6/4/2020.
//

#pragma once

#include "common.h"
#include "platform.h"
#include "widget.h"
#include "font.h"
#include "shader.h"

class ChapterNumber : public Widget {
public:
    ChapterNumber();
    explicit ChapterNumber(Widget* parent);
    void onCodepoint(const Event::Codepoint& codepoint);
    void onMacro(const Event::Macro& macro);
    void onWindowResize(const Event::WindowResize& resize);
    void draw(Draw::CallQueue& draw_buffer);
    void update();
private:
    const Font* m_font = nullptr;
    const Shader* m_shader = nullptr;
    Draw::Context m_draw_context;
};
