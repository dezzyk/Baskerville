//
// Created by Feed on 7/20/2020.
//

#pragma once

#include "common.h"
#include "entt/entt.hpp"

class App {
public:
    void startup();
    void shutdown();
    void update();
    void draw();
private:
    entt::registry m_registry;
};
