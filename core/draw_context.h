//
// Created by Feed on 6/5/2020.
//

#pragma once

#include "common.h"

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
        void upload(u32 size, void* data) const;
        b32 valid() const;

    private:
        std::optional<u32> m_vbo;
        std::optional<u32> m_vao;
    };

}