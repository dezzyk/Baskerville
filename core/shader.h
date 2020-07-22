//
// Created by Feed on 5/31/2020.
//

#pragma once

#include "common.h"
#include "shader_src.h"

//#include "glad/glad.h"

#include <string>
#include <optional>
#include <unordered_map>

class Shader {
public:
    class Cache {
    public:
        static const Shader* load(std::string shader_name, std::string vert, std::string frag);
        static const Shader* fetch(std::string shader_name);
        void clear();
    private:
        static std::unordered_map<std::string, Shader> m_shader_cache;
    };
    Shader();
    Shader(Shader&& other) noexcept;
    ~Shader();
    Shader(const Shader&) =delete;
    Shader& operator=(const Shader&) =delete;
    const std::optional<u32>& getHandle() const;
    const std::optional<u32>& getMatricesDescriptorBlockIndex();
private:
    Shader(std::string vert, std::string frag);
    std::optional<u32> m_handle;
    std::optional<u32> m_matrices_descriptor_block_index;
};
