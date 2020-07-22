//
// Created by Feed on 7/22/2020.
//

#pragma once

#include "common.h"
#include "renderable.h"

struct Draw {
    Draw() : renderable(nullptr), shader(nullptr) {}
    Draw(const Renderable* r, const Shader* s) : renderable(r), shader(s) {}
    const Renderable* const renderable;
    const Shader* const shader;
};
