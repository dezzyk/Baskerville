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
    void update();
    void draw(std::vector<Draw>& queue);
private:
    entt::registry m_registry;
    entt::entity test;
};
