//
// Created by Feed on 6/4/2020.
//

#pragma once

#include "common.h"
#include "platform.h"
#include "widget.h"
#include "font.h"
#include "shader.h"

#include <optional>

class SingleChar : public Widget {
public:
    SingleChar();
    explicit SingleChar(Widget* parent);
    void setValue(u32 codepoint);
    void setValue(char codepoint);
    void onWindowResize() override ;
    void draw(Draw::CallQueue& draw_buffer) override ;
private:
    Draw::Context m_draw_context;
    std::optional<u32> m_codepoint;
    const Font* m_font = nullptr;
    const Shader* m_shader = nullptr;
};
