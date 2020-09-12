//
// Created by Feed on 7/22/2020.
//

#pragma once

#include "common.h"
#include "label.h"
#include "event.h"

#include "entt/entt.hpp"

#include <array>

struct Editor {
    std::array<entt::entity, 6> label_entitys;
    static void handleText(const Event::TextInput& text, entt::registry& reg);
    static void handleMacro(const Event::Macro& macro, entt::registry& reg);
    static entt::entity create(entt::registry& reg);
};
