//
// Created by Feed on 5/31/2020.
//

#include "shader.h"
#include "gl_err.h"

#include "glad/glad.h"

#include <iostream>

const Shader* Shader::Cache::load(std::string shader_name, std::string vert, std::string frag) {
    if(m_shader_cache.find(shader_name) == m_shader_cache.end()) {
        m_shader_cache.emplace(shader_name, Shader(vert, frag));
    }
    return &m_shader_cache[shader_name];
}

const Shader* Shader::Cache::fetch(std::string shader_name) {
    std::cout << m_shader_cache.size() << std::endl;
    if(m_shader_cache.find(shader_name) != m_shader_cache.end()) {
        return &m_shader_cache[shader_name];
    }
    return nullptr;
}

void Shader::Cache::clear() {
    m_shader_cache.clear();
}

std::unordered_map<std::string, Shader> Shader::Cache::m_shader_cache;

Shader::Shader(std::string vert, std::string frag) {
    u32 vert_handle, frag_handle, program_handle;
    vert_handle = glCreateShader(GL_VERTEX_SHADER); CheckGLError();
    frag_handle = glCreateShader(GL_FRAGMENT_SHADER); CheckGLError();
    const char* vert_c_str = vert.c_str(); CheckGLError();
    const char* frag_c_str = frag.c_str(); CheckGLError();
    glShaderSource(vert_handle, 1, &vert_c_str, NULL); CheckGLError();
    glShaderSource(frag_handle, 1, &frag_c_str, NULL); CheckGLError();
    glCompileShader(vert_handle); CheckGLError();
    glCompileShader(frag_handle); CheckGLError();
    program_handle = glCreateProgram(); CheckGLError();
    glAttachShader(program_handle, vert_handle); CheckGLError();
    glAttachShader(program_handle, frag_handle); CheckGLError();
    glLinkProgram(program_handle); CheckGLError();
    int success;
    char info_log[512];
    glGetProgramiv(program_handle, GL_LINK_STATUS, &success); CheckGLError();
    if (!success) {
        glGetProgramInfoLog(program_handle, 512, NULL, info_log);
        std::cout << "Failed to create shader program \n" << info_log << std::endl;
    } else {
        m_handle = program_handle;
    }
    glDeleteShader(vert_handle); CheckGLError();
    glDeleteShader(frag_handle); CheckGLError();
}

Shader::Shader() {}

Shader::Shader(Shader&& other) noexcept : m_handle(other.m_handle) {
    other.m_handle = std::nullopt;
}

Shader::~Shader() {
    if(m_handle.has_value()) {
        glUseProgram(0); CheckGLError();
        glDeleteProgram(m_handle.value()); CheckGLError();
        m_handle = std::nullopt;
    }
}

const std::optional<u32>& Shader::getHandle() const {
    return m_handle;
}