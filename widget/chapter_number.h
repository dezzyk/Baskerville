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
    void update() override ;
    void onCodepoint(const Event::Codepoint& codepoint) override ;
    void onMacro(const Event::Macro& macro) override ;
    void onWindowResize() override ;
    void draw(Draw::CallQueue& draw_buffer) override ;
private:
    const Font* m_font = nullptr;
    const Shader* m_shader = nullptr;
    Draw::Context m_draw_context;
};
