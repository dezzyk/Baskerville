//
// Created by Feed on 5/27/2020.
//

#define pragma once

#include "common.h"

#include <string>
#include <optional>
#include <vector>

// Only standard ASCII printable range, extended is not included.
// glyph indeces 33-126 stored in vector;
// glyph indeces 33-126 compiled into texture atlas;

class Font {
public:
    struct Glyph {

    };
    Font();
    Font(std::string font_name);
    ~Font();
    b32 isCompiled();
    b32 getGlyph(u32 index, Glyph& glyph);
    const std::optional<u32>& getTextureHandle();
private:
    std::vector<Glyph> m_glyphs;
    std::optional<u32> m_texture_handle;
    b32 m_compiled = false;
};
