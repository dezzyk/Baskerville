//
// Created by Feed on 5/29/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "draw.h"
#include "editor.h"

#include "glm/glm.hpp"

class Root : public Widget {
public:
    Root();
    void update() override ;
    void onCodepoint(const Event::Codepoint& codepoint) override ;
    void onMacro(const Event::Macro& macro) override ;
    void onWindowResize() override ;
    void draw(Draw::CallQueue& draw_buffer) override ;
private:
    Draw::Context m_draw_context;
    const Shader* m_shader = nullptr;
    Editor m_editor;
};
