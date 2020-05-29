//
// Created by Feed on 5/27/2020.
//

#define pragma once

#include "common.h"

#include <string>
#include <optional>
#include <vector>
#include "stb/stb_truetype.h"
#include "glm/glm.hpp"

// Only standard ASCII printable range, extended is not included.
// glyph indeces 33-126 stored in vector;
// glyph indeces 33-126 compiled into texture atlas;

class Font {
public:
    struct Glyph {

    };
    Font();
    Font(std::string font_name, u32 w, u32 h, u32 pixel_height);
    ~Font();
    b32 isCompiled();
    b32 getGlyph(u32 index, Glyph& glyph);
    const std::optional<u32>& getTextureHandle();
    f32 getScale();
    f32 getBaseline();
    glm::vec2 getBitmapSize();
private:
    std::vector<unsigned char> generateBitmap(stbtt_fontinfo& font_info, u32 codepoint);
    std::vector<Glyph> m_glyphs;
    std::optional<u32> m_texture_handle;
    f32 m_scale = 0.0f;
    f32 m_baseline = 0.0f;
    u32 m_w = 0;
    u32 m_h = 0;
    u32 m_pixel_height = 0;
    b32 m_compiled = false;
};
