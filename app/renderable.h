//
// Created by Feed on 7/20/2020.
//

#pragma once

#include "common.h"
#include "type_index.h"
#include "shader.h"
#include "vertex.h"

#include "glm/glm.hpp"
#include "glad/glad.h"

#include <array>

class Renderable {
public:

    struct Descriptor {

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
            type = TypeIndex::make<glm::mat4>();
            value.mat4 = new_value;
        }

        template<>
        void setValue<glm::vec4>(glm::vec4 new_value) {
            type = TypeIndex::make<glm::vec4>();
            value.vec4 = new_value;
        }

        template<>
        void setValue<glm::vec3>(glm::vec3 new_value) {
            type = TypeIndex::make<glm::vec3>();
            value.vec3 = new_value;
        }

        template<>
        void setValue<glm::vec2>(glm::vec2 new_value) {
            type = TypeIndex::make<glm::vec2>();
            value.vec2 = new_value;
        }

        template<>
        void setValue<f32>(f32 new_value) {
            type = TypeIndex::make<f32>();
            value.u_f32 = new_value;
        }

        template<>
        void setValue<u32>(u32 new_value) {
            type = TypeIndex::make<u32>();
            value.u_u32 = new_value;
        }

        template<>
        void setValue<i32>(i32 new_value) {
            type = TypeIndex::make<i32>();
            value.u_i32 = new_value;
        }

        template<>
        void setValue<Texture>(Texture new_value) {
            type = TypeIndex::make<Texture>();
            value.texture = new_value;
        }

        template<>
        void setValue<ArrayTexture>(ArrayTexture new_value) {
            type = TypeIndex::make<ArrayTexture>();
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

        TypeIndex type;
        Value value;
    };

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

    Renderable(Renderable&& other) noexcept;
    Renderable& operator=(Renderable&& other);
    ~Renderable();

    const std::optional<u32>& getVBO() const;
    const std::optional<u32>& getVAO() const;
    const Shader* getShader() const;
    void quadUpload(Quad& data);
    void quadUpload(std::vector<Quad>& data);
    template <int C>
    void quadUpload(std::array<Quad, C>& data) {
        if(valid()) {
            m_size = data.size();
            u32 byte_size = m_size * sizeof(Quad);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
            if(byte_size > m_capacity) {
                m_capacity = byte_size + (byte_size % sizeof(Quad)) + (sizeof(Quad) * 8);
                glBufferData(GL_ARRAY_BUFFER, m_capacity, nullptr, GL_DYNAMIC_DRAW);
            }
            glBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, data.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
    u32 size() const;
    void clear();
    b32 valid() const;

    glm::mat4 model = glm::mat4(1.0f);
    std::array<Descriptor, 6> descriptors;

protected:
    Renderable();
    Renderable(const char* shader_name);
    Renderable(const Renderable&) =delete;
    Renderable& operator=(const Renderable&) =delete;

    const Shader* m_shader = nullptr;
    std::optional<u32> m_vbo;
    std::optional<u32> m_vao;
    u32 m_capacity = 0;
    u32 m_size = 0;
    u32 m_ubo_index = 0;

private:

};

using RenderableQueue = std::vector<const Renderable* const>;
