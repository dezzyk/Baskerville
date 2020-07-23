//
// Created by Feed on 7/22/2020.
//

#pragma once

#include "common.h"
#include "font.h"
#include "draw.h"

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include <string>

struct Label {
    std::string value = "";
    glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f};
    const Font* font = nullptr;
    u32 pixel_height = 0;
    static void draw(entt::entity entity, entt::registry &reg, std::vector<Draw>& queue);
};