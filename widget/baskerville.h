//
// Created by Feed on 5/29/2020.
//

#pragma once

#include "common.h"
#include "widget.h"

#include "glm/glm.hpp"

class Baskerville : public Widget {
public:
    void handleCodepoint(Event::Codepoint codepoint);
    void handleMacro(Event::Macro macro);
    void handleResize(Event::Resize value);
    void draw();
private:
    glm::vec3 m_canvas_color = { 1.0f, 1.0f, 1.0f};
};
