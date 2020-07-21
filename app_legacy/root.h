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
#include "menu.h"

#include "glm/glm.hpp"

#include <string>
#include <memory>

class Root : public Widget, public Event::Handler {
public:
    Root(CacheBank& cache);
    Draw::RedrawFlag onTextInput(const Event::TextInput& text) override ;
    Draw::RedrawFlag onMacro(const Event::Macro& macro) override ;
    Draw::RedrawFlag onMouseClick(Event::MouseClick mouse_click) override ;

private:
    Draw::RedrawFlag derivedUpdate(f64 delta) override ;
    void derivedDraw(Draw::Queue& queue) override ;

    Draw::Context m_draw_context;
    std::unique_ptr<Editor> m_editor;
    std::unique_ptr<Menu> m_menu;

};
