//
// Created by Feed on 5/29/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "draw.h"
#include "editor.h"
#include "project.h"
#include "cache.h"

#include "glm/glm.hpp"

#include <string>

class Root : public Widget, public Event::Handler {
public:
    Root(CacheBank& cache);
    Draw::RedrawFlag onTextInput(const Event::TextInput& text) override ;
    Draw::RedrawFlag onMacro(const Event::Macro& macro) override ;
    Draw::RedrawFlag onMouseClick(Event::MouseClick mouse_click) override ;

private:
    Draw::RedrawFlag derivedUpdate(f64 delta) override ;
    void derivedDraw(Draw::CallQueue& draw_buffer, f32 scale) override ;

    Draw::Context m_draw_context;
    const Shader* m_shader = nullptr;
    Editor m_editor;

};
