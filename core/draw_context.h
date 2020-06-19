//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"
#include "draw_box.h"

#include "glad/glad.h"

#include <vector>
#include <array>
#include <optional>

namespace Draw {

    class Context {
    public:
        Context();
        Context(Context&& other) noexcept;
        Context(const Context&) =delete;
        Context& operator=(const Context&) =delete;
        ~Context();
        const std::optional<u32> &getVBO() const;
        const std::optional<u32> &getVAO() const;
        void boxUpload(Draw::Box& data);
        void boxUpload(std::vector<Draw::Box>& data);
        template <int C>
        void boxUpload(std::array<Draw::Box, C>& data) {
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
        u32 size();
        void clear();
        b32 valid() const;

    private:
        std::optional<u32> m_vbo;
        std::optional<u32> m_vao;
        u32 m_capacity = 0;
        u32 m_size = 0;
    };

}