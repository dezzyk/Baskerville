//
// Created by Feed on 6/5/2020.
//

#include "draw_context.h"

#include <iostream>

Draw::Context::Context() {
    u32 vbo_handle, vao_handle;
    glGenBuffers(1, &vbo_handle);
    glGenVertexArrays(1, &vao_handle);
    if(vbo_handle != 0) {
        if(vao_handle != 0) {
            m_capacity = sizeof(Draw::Box) * 8;  // Preallocate for 8 boxes
            m_vbo = vbo_handle;
            m_vao = vao_handle;
            glBindVertexArray(m_vao.value());
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
            glBufferData(GL_ARRAY_BUFFER, m_capacity, nullptr, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(4* sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7* sizeof(float)));
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
        } else {
            glDeleteBuffers(1, &vbo_handle);
        }
    }
    if(!valid()) {
        std::cout << "Failed to create draw context" << std::endl;
    }
}

Draw::Context::Context(Context&& other) noexcept :
    m_vbo(other.m_vbo),
    m_vao(other.m_vao) {
    other.m_vbo = std::nullopt;
    other.m_vao = std::nullopt;
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

void Draw::Context::boxUpload(Draw::Box& data) {
    if(valid()) {
        m_size = 1;
        u32 byte_size = sizeof(Draw::Box);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
        if(byte_size > m_capacity) { // Should never happen
            m_capacity = byte_size + (byte_size % sizeof(Draw::Box)) + (sizeof(Draw::Box) * 8);
            glBufferData(GL_ARRAY_BUFFER, m_capacity, nullptr, GL_DYNAMIC_DRAW);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, &data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Draw::Context::boxUpload(std::vector<Draw::Box>& data) {
    if(valid()) {
        m_size = data.size();
        u32 byte_size = m_size * sizeof(Draw::Box);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
        if(byte_size > m_capacity) {
            m_capacity = byte_size + (byte_size % sizeof(Draw::Box)) + (sizeof(Draw::Box) * 8);
            glBufferData(GL_ARRAY_BUFFER, m_capacity, nullptr, GL_DYNAMIC_DRAW);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, data.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

u32 Draw::Context::size() {
    return m_size;
}

void Draw::Context::clear() {
    m_size = 0;
}

b32 Draw::Context::valid() const {
    return (m_vao.has_value() && m_vbo.has_value());
}