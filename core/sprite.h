//
// Created by Feed on 5/26/2020.
//

#define pragma once

#include "common.h"

#include "glm/glm.hpp"

struct Sprite {
    glm::vec4 position = { 0.0f, 0.0f, 0.0f, 0.0f}; // x,y == position | z,w == uv offset
    u32 page = 0;
};