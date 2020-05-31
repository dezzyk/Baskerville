//
// Created by Feed on 5/31/2020.
//

#pragma once

#include "shader.h"
#include "meta.h"

#include "glm/glm.hpp"

#include <array>
#include <optional>

struct Uniform {
    struct Texture { u32 handle = 0; u32 index = 0;};
    struct ArrayTexture { u32 handle = 0; u32 index = 0;};
    template <typename T>
    void setValue(T new_value) {
        // If set to an invalid uniform value, set type to empty for checking against
        type = Meta::MakeType<Meta::Empty>();
    }
    template <>
    void setValue<glm::mat4>(glm::mat4 new_value) {
        type = Meta::MakeType<glm::mat4>();
        value.mat4 = new_value;
    }
    template <>
    void setValue<glm::vec4>(glm::vec4 new_value) {
        type = Meta::MakeType<glm::vec4>();
        value.vec4 = new_value;
    }
    template <>
    void setValue<glm::vec3>(glm::vec3 new_value) {
        type = Meta::MakeType<glm::vec3>();
        value.vec3 = new_value;
    }
    template <>
    void setValue<glm::vec2>(glm::vec2 new_value) {
        type = Meta::MakeType<glm::vec2>();
        value.vec2 = new_value;
    }
    template <>
    void setValue<Texture>(Texture new_value) {
        type = Meta::MakeType<Texture>();
        value.texture = new_value;
    }
    template <>
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

struct Draw {
    // Uniform 0 will always be the projection matrix
    std::array<Uniform, 7> uniforms;
    std::array<Buffer, 4> buffers;
    const Shader* shader = nullptr;
    std::optional<u32> vao;
    u32 count = 0;
};
