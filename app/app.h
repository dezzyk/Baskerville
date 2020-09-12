//
// Created by Feed on 7/20/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "debug.h"
#include "draw.h"
#include "platform.h"

#include "entt/entt.hpp"

class App {
public:
    void startup();
    void shutdown();
    void update(f64 delta);
    void onTextInput(const Event::TextInput& text);
    void onMacro(const Event::Macro& macro);
    void onMouseClick(const Event::MouseClick& mouse);
    void draw(std::vector<Draw>& queue);
private:
    entt::registry m_registry;
    entt::entity test;
    b32 m_show_debug = false;
};
