//
// Created by Feed on 5/27/2020.
//

#pragma once

#include "common.h"

#include <array>
#include <string>
#include <optional>
#include <vector>
#include <unordered_map>

#include "stb/stb_truetype.h"
#include "glm/glm.hpp"

// Only standard ASCII printable range should be default (33-126), no need to load full extended range

class Font {
public:
    class Cache {
    public:
        static const Font* load(std::string font_name,
                std::string filename,
                u32 pixel_height);
        static const Font* fetch(std::string font_name);
        void clear();
    private:
        static std::unordered_map<std::string, Font> m_font_cache;
    };
    struct Glyph {
        i32 index = -1;
        i32 layer = -1;
        struct {
            i32 x0 = 0;
            i32 y0 = 0;
            i32 x1 = 0;
            i32 y1 = 0;
        } bounding_box;
    };
    Font();
    Font(Font&& other) noexcept;
    ~Font();
    Font(const Font&) =delete;
    Font& operator=(const Font&) =delete;
    const std::optional<u32>& getHandle() const;
    f32 calcScale(u32 pixel_height) const;
    //glm::vec2 getBitmapSize() const;
    glm::vec2 getBoundingBoxSize() const;
    u32 getPixelHeight() const;
    i32 getKernAdvance(u32 pixel_height, u32 c0, u32 c1) const;
    void getCodepointAdvance(u32 pixel_height, u32 codepoint, i32& advance, i32& lsb) const;
    u32 calcStringPixelWidth(std::string& str, u32 pixel_height) const;
    i32 getCodepointLayer(u32 codepoint) const;
    i32 getSpecificGlyphLayer(u32 index) const;
    b32 generateBitmap(u32 index, std::vector<unsigned char>& data);
private:
    Font(std::string font_name, std::string filename, u32 pixel_height);
    b32 loadFromCache(std::string font_name);
    b32 loadFromFile(std::string font_name, std::string filename);
    stbtt_fontinfo m_font_info;
    std::vector<unsigned char> m_font_data;
    std::vector<Glyph> m_glyphs;
    std::array<i32, 255> m_codepoints;
    std::optional<u32> m_texture_handle;
    glm::vec2 m_bitmap_size;
    glm::vec4 m_bounding_box;
    f32 m_scale =1.0f;
    f32 m_baseline = 0.0f;
    u32 m_pixel_height = 0;
    f32 m_msdf_pixel_range = 8.0f;
};
