//
// Created by Feed on 6/5/2020.
//

#include "draw_context.h"

#include <iostream>

Draw::Context::Context() {}

Draw::Context::Context(const char* shader_name) {
    m_shader = Shader::Cache::fetch(shader_name);
    if(m_shader) {
        u32 vbo_handle, vao_handle;
        glGenBuffers(1, &vbo_handle);
        glGenVertexArrays(1, &vao_handle);
        if (vbo_handle != 0) {
            if (vao_handle != 0) {
                m_capacity = sizeof(Draw::Quad) * 8;  // Preallocate for 8 boxes
                m_vbo = vbo_handle;
                m_vao = vao_handle;
                glBindVertexArray(m_vao.value());
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
                glBufferData(GL_ARRAY_BUFFER, m_capacity, nullptr, GL_DYNAMIC_DRAW);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) 0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (4 * sizeof(float)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (7 * sizeof(float)));
                glEnableVertexAttribArray(2);
                glBindVertexArray(0);

                m_ubo_index = glGetUniformBlockIndex(m_shader->getHandle().value(), "matrices");
                if(m_ubo_index == GL_INVALID_INDEX) {
                    m_ubo_index = 0;
                    glDeleteVertexArrays(1, &m_vao.value());
                    glDeleteBuffers(1, &m_vbo.value());
                    m_vbo = std::nullopt;
                    m_vao = std::nullopt;
                    m_shader = nullptr;
                }
            } else {
                glDeleteBuffers(1, &vbo_handle);
                m_shader = nullptr;
            }
        }
    }
    if(!valid()) {
        std::cout << "Failed to create draw context" << std::endl;
    }
}

Draw::Context& Draw::Context::operator=(Context&& other) {
    m_vbo = other.m_vbo;
    m_vao = other.m_vao;
    m_shader = other.m_shader;
    other.m_vbo = std::nullopt;
    other.m_vao = std::nullopt;
    other.m_shader = nullptr;
    return *this;
}

Draw::Context::Context(Context&& other) noexcept :
    m_vbo(other.m_vbo),
    m_vao(other.m_vao),
    m_shader(other.m_shader){
    other.m_vbo = std::nullopt;
    other.m_vao = std::nullopt;
    other.m_shader = nullptr;
}

Draw::Context::~Context() {
    if(m_vao.has_value()){
        glDeleteVertexArrays(1, &m_vao.value());
    }
    if(m_vbo.has_value()){
        glDeleteBuffers(1, &m_vbo.value());
    }
}

const std::optional<u32>& Draw::Context::getVBO() const {
    return m_vbo;
}

const std::optional<u32>& Draw::Context::getVAO() const {
    return m_vao;
}

const Shader* Draw::Context::getShader() const {
    return m_shader;
}

void Draw::Context::quadUpload(Draw::Quad& data) {
    if(valid()) {
        m_size = 1;
        u32 byte_size = sizeof(Draw::Quad);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
        if(byte_size > m_capacity) { // Should never happen
            m_capacity = byte_size + (byte_size % sizeof(Draw::Quad)) + (sizeof(Draw::Quad) * 8);
            glBufferData(GL_ARRAY_BUFFER, m_capacity, nullptr, GL_DYNAMIC_DRAW);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, &data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Draw::Context::quadUpload(std::vector<Draw::Quad>& data) {
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

u32 Draw::Context::size() const {
    return m_size;
}

void Draw::Context::clear() {
    m_size = 0;
}

b32 Draw::Context::valid() const {
    return (m_vao.has_value() && m_vbo.has_value() && m_shader);
}