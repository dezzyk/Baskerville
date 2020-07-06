//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"
#include "draw_vertex.h"

#include <array>

namespace Draw {

    struct Quad {
        void setColor(glm::vec4 color) {
            for(auto& vert : vertices) {
                vert.color = color;
            }
        }

        void setUVDepth(u32 depth) {
            for(auto& vert : vertices) {
                vert.uv.z = depth;
            }
        }

        void operator*(glm::mat4 model) {
            vertices[0].pos = model * glm::vec4(vertices[0].pos.x, vertices[0].pos.y, 0.0f, 1.0f);
            vertices[1].pos = model * glm::vec4(vertices[1].pos.x, vertices[1].pos.y, 0.0f, 1.0f);
            vertices[2].pos = model * glm::vec4(vertices[2].pos.x, vertices[2].pos.y, 0.0f, 1.0f);
            vertices[3].pos = model * glm::vec4(vertices[3].pos.x, vertices[3].pos.y, 0.0f, 1.0f);
            vertices[4].pos = model * glm::vec4(vertices[4].pos.x, vertices[4].pos.y, 0.0f, 1.0f);
            vertices[5].pos = model * glm::vec4(vertices[5].pos.x, vertices[5].pos.y, 0.0f, 1.0f);
        }

        Quad &operator=(const Quad &other) {
            vertices = other.vertices;
            return *this;
        }

        Quad &operator*=(glm::mat4 model) {
            operator*(model);
            return *this;
        }

        std::array<Vertex, 6> vertices = {{
            {{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.5f, 0.5f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, -0.5f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-0.5f, 0.5f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, -0.5f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {-0.5f, -0.5f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-0.5f, 0.5f}}
                                          }};
    };

}