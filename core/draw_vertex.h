//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"

#include "glm/glm.hpp"

namespace Draw {

    struct Vertex {
        glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec3 uv = {0.0f, 0.0f, 0.0f};
        glm::vec2 pos = {0.0f, 0.0f};
    };

}
