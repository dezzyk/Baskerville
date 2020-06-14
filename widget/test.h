//
// Created by Feed on 6/14/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "font.h"
#include "draw.h"

#include <string>

class Test : public Widget {
public:
    Test();
    explicit Test(Widget* parent);
    void onWindowResize() override ;
    void draw(Draw::CallQueue& draw_buffer) override ;
private:
    std::string m_value;
    Draw::Context m_draw_context;
    const Shader* m_shader = nullptr;
    const Font* m_font = nullptr;
};
