//
// Created by Feed on 6/5/2020.
//

#include "draw_context.h"

#include "glad/glad.h"

#include <iostream>

Draw::Context::Context() {
    u32 vbo_handle, vao_handle;
    glGenBuffers(1, &vbo_handle);
    glGenVertexArrays(1, &vao_handle);
    if(vbo_handle != 0) {
        if(vao_handle != 0) {
            m_vbo = vbo_handle;
            m_vao = vao_handle;
            glBindVertexArray(m_vao.value());
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
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

void Draw::Context::upload(u32 size, void* data) const {
    if(valid()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

b32 Draw::Context::valid() const {
    return (m_vao.has_value() && m_vbo.has_value());
}