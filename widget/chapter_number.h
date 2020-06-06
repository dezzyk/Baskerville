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
    ChapterNumber(Widget* parent);
    void draw(Draw::CallQueue& draw_buffer) override ;
private:
    const Font* m_font = nullptr;
    const Shader* m_shader = nullptr;
    Draw::Handles m_draw_handles;
};
