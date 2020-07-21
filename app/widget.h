//
// Created by Feed on 7/20/2020.
//

#pragma once

#include "common.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include <optional>

struct Widget {
    enum class Align {
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center
    };
    glm::vec2 size = { 1.0f, 1.0f };
    glm::vec2 offset = { 0.0f, 0.0f };
    b32 unscaled_width = false;
    b32 unscaled_height = false;
    Align alignment = Align::Center;
    std::optional<entt::entity> parent;

};

namespace WidgetProcs {
    std::optional<glm::vec2> calcDrawPos(entt::entity entity, entt::registry &reg);
    std::optional<glm::vec2> calcDrawSize(entt::entity entity, entt::registry &reg);

}