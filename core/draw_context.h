//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"
#include "draw_quad.h"
#include "shader.h"
#include "meta.h"

#include "glad/glad.h"

#include <vector>
#include <array>
#include <optional>

namespace Draw {

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

    class Context {
    public:
        Context();
        Context(const char* shader_name);
        Context(Context&& other) noexcept;
        Context(const Context&) =delete;
        Context& operator=(const Context&) =delete;
        Context& operator=(Context&& other);
        ~Context();
        const std::optional<u32>& getVBO() const;
        const std::optional<u32>& getVAO() const;
        const Shader* getShader() const;
        void quadUpload(Draw::Quad& data);
        void quadUpload(std::vector<Draw::Quad>& data);
        template <int C>
        void quadUpload(std::array<Draw::Quad, C>& data) {
            if(valid()) {
                m_size = data.size();
                u32 byte_size = m_size * sizeof(Draw::Quad);
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
                if(byte_size > m_capacity) {
                    m_capacity = byte_size + (byte_size % sizeof(Draw::Quad)) + (sizeof(Draw::Quad) * 8);
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
        std::array<Uniform, 6> uniforms;
        std::array<Buffer, 4> buffers;

    private:
        const Shader* m_shader = nullptr;
        std::optional<u32> m_vbo;
        std::optional<u32> m_vao;
        u32 m_capacity = 0;
        u32 m_size = 0;
    };

    using Queue = std::vector<const Draw::Context*>;

}