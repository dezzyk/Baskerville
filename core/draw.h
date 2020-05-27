//
// Created by Feed on 5/26/2020.
//

#define pragma once

#include "common.h"
#include "sprite.h"

#include "glm/glm.hpp"

#include <vector>
#include <optional>

struct Draw {
    glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 position = {0.0f, 0.0f, 0.0f, 0.0f}; // x,y == position | z,w == uv offset
    u32 page;
};
