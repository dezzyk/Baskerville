//
// Created by Feed on 5/27/2020.
//

#define pragma once

#include "common.h"

#include <string>
#include <optional>
#include <vector>
#include <unordered_map>

#include "stb/stb_truetype.h"
#include "glm/glm.hpp"

// Only standard ASCII printable range, extended is not included.
// glyph indeces 33-126 stored in vector;
// glyph indeces 33-126 compiled into texture atlas;

class Font {
public:
    class Cache {
    public:
        static const Font &load(std::string font_name, std::string filename, u32 w, u32 h, u32 pixel_height);
        static void clear();
    private:
        static std::unordered_map<std::string, Font> m_font_cache;
    };
    const std::optional<u32>& getTextureHandle();
    f32 getScale();
    f32 getBaseline();
    glm::vec2 getBitmapSize();
    u32 getKernOffset(u32 c0, u32 c1);
    std::vector<unsigned char> generateBitmap(u32 codepoint);
    Font();
    Font(Font&& other) noexcept;
    ~Font();
    //friend class std::unordered_map<std::string, Font>;
    Font(const Font&) =delete;
    Font& operator=(const Font&) =delete;
private:
    Font(std::string font_name, u32 w, u32 h, u32 pixel_height);
    stbtt_fontinfo m_font_info;
    std::optional<u32> m_texture_handle;
    f32 m_scale = 0.0f;
    f32 m_baseline = 0.0f;
    u32 m_w = 0;
    u32 m_h = 0;
    u32 m_pixel_height = 0;
};
