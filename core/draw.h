//
// Created by Feed on 5/31/2020.
//

#pragma once

#include "shader.h"
#include "meta.h"

#include "glm/glm.hpp"

#include <array>
#include <optional>
#include <iostream>

namespace Draw {

    struct Vertex {
        glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec3 uv = {0.0f, 0.0f, 0.0f};
        glm::vec2 pos = {0.0f, 0.0f};
    };

    struct Box {
        void setColor(glm::vec4 color) {
            for(auto& vert : vertices) {
                vert.color = color;
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

        Box &operator=(const Box &other) {
            vertices = other.vertices;
            return *this;
        }

        Box &operator*=(glm::mat4 model) {
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

    struct Uniform {
        struct Texture {
            Texture() : handle(0), index(0) {}

            Texture(u32 h, u32 i) : handle(h), index(i) {}

            u32 handle;
            u32 index;
        };

        struct ArrayTexture {
            ArrayTexture() : handle(0), index(0) {}

            ArrayTexture(u32 h, u32 i) : handle(h), index(i) {}

            u32 handle;
            u32 index;
        };

        template<typename T>
        void setValue(T new_value) {
            // If set to an invalid uniform value, set type to empty for checking against
            type = Meta::MakeType<Meta::Empty>();
            std::cout << "Invalid uniform value assigned, defaulted to empty" << std::end;
        }

        template<>
        void setValue<glm::mat4>(glm::mat4 new_value) {
            type = Meta::MakeType<glm::mat4>();
            value.mat4 = new_value;
        }

        template<>
        void setValue<glm::vec4>(glm::vec4 new_value) {
            type = Meta::MakeType<glm::vec4>();
            value.vec4 = new_value;
        }

        template<>
        void setValue<glm::vec3>(glm::vec3 new_value) {
            type = Meta::MakeType<glm::vec3>();
            value.vec3 = new_value;
        }

        template<>
        void setValue<glm::vec2>(glm::vec2 new_value) {
            type = Meta::MakeType<glm::vec2>();
            value.vec2 = new_value;
        }

        template<>
        void setValue<f32>(f32 new_value) {
            type = Meta::MakeType<f32>();
            value.u_f32 = new_value;
        }

        template<>
        void setValue<u32>(u32 new_value) {
            type = Meta::MakeType<u32>();
            value.u_u32 = new_value;
        }

        template<>
        void setValue<i32>(i32 new_value) {
            type = Meta::MakeType<i32>();
            value.u_i32 = new_value;
        }

        template<>
        void setValue<Texture>(Texture new_value) {
            type = Meta::MakeType<Texture>();
            value.texture = new_value;
        }

        template<>
        void setValue<ArrayTexture>(ArrayTexture new_value) {
            type = Meta::MakeType<ArrayTexture>();
            value.array_texture = new_value;
        }

        union Value {
            Value() {}

            glm::mat4 mat4;
            glm::vec4 vec4;
            glm::vec3 vec3;
            glm::vec2 vec2;
            f32 u_f32;
            u32 u_u32;
            i32 u_i32;
            Texture texture;
            ArrayTexture array_texture;
        };

        Meta::Type type;
        Value value;
    };

    struct Buffer {
        Buffer() : handle(0) {}
        Buffer(u32 h, u32 i) : handle(h) {}
        const u32 handle;
    };

    struct Handles {
        std::optional<u32> vbo;
        std::optional<u32> vao;
    };

    struct Call {
        // Uniform 0 will always be the projection matrix, total of 8 uniforms including 0
        std::array<Uniform, 7> uniforms;
        std::array<Buffer, 4> buffers;
        const Shader* shader = nullptr;
        Handles handles;
        u32 count = 0;
    };

    using CallQueue = std::vector<Call>;

}