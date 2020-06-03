//
// Created by Feed on 5/27/2020.
//

#pragma once

#include "common.h"

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
                u32 start_codepoint,
                u32 end_codepoint,
                u32 pixel_height);
        static const Font* fetch(std::string font_name);
        void clear();
    private:
        static std::unordered_map<std::string, Font> m_font_cache;
    };
    Font();
    Font(Font&& other) noexcept;
    ~Font();
    Font(const Font&) =delete;
    Font& operator=(const Font&) =delete;
    const std::optional<u32>& getHandle() const;
    //f32 getScale() const;
    f32 getBaseline() const;
    glm::vec2 getBitmapSize() const;
    glm::vec2 getBoundingBoxSize() const;
    u32 getKernOffset(u32 c0, u32 c1) const;
    u32 getStartCodepoint() const;
    u32 getEndCodepoint() const;
    u32 getPixelHeight() const;
    f32 getMSDFPixelRange() const;
    std::vector<unsigned char> generateBitmap(u32 codepoint);
private:
    Font(std::string font_name, u32 start_codepoint, u32 end_codepoint, u32 pixel_height);
    stbtt_fontinfo m_font_info;
    std::optional<u32> m_texture_handle;
    glm::vec2 m_bitmap_size;
    glm::vec2 m_bounding_box_size;
    f32 m_scale = 0.0f;
    f32 m_baseline = 0.0f;
    u32 m_start_codepoint = 0;
    u32 m_end_codepoint = 0;
    u32 m_pixel_height = 0;
    f32 m_msdf_pixel_range = 8.0f;
};
