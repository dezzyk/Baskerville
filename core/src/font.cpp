//
// Created by Feed on 5/27/2020.
//

#include "font.h"
#include "data_path.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"
#include "stb/stb_image_write.h"
#include "msdfgen/msdfgen.h"
#include "glad/glad.h"

#include "gl_err.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <thread>
#include <future>

const Font* Font::Cache::load(std::string font_name,
                                std::string filename,
                                u32 start_codepoint,
                                u32 end_codepoint,
                                u32 pixel_height) {
    if(m_font_cache.find(font_name) == m_font_cache.end()) {
        m_font_cache.emplace(font_name, Font(filename, start_codepoint, end_codepoint, pixel_height));
    }
    return &m_font_cache[font_name];
}

const Font* Font::Cache::fetch(std::string font_name) {
    if(m_font_cache.find(font_name) != m_font_cache.end()) {
        return &m_font_cache[font_name];
    }
    return nullptr;
}

void Font::Cache::clear() {
    m_font_cache.clear();
}

std::unordered_map<std::string, Font> Font::Cache::m_font_cache;

Font::Font() {}

Font::Font(Font&& other) noexcept :
        m_font_info(other.m_font_info),
        m_font_data(std::move(other.m_font_data)), // Must move to ensure data pointer isnt broken
        m_texture_handle(other.m_texture_handle),
        m_baseline(other.m_baseline),
        m_bitmap_size(other.m_bitmap_size),
        m_bounding_box(other.m_bounding_box),
        m_start_codepoint(other.m_start_codepoint),
        m_end_codepoint(other.m_end_codepoint),
        m_pixel_height(other.m_pixel_height),
        m_msdf_pixel_range(other.m_msdf_pixel_range),
        m_glyphs(other.m_glyphs),
        m_space_glyph(other.m_space_glyph),
        m_scale(other.m_scale)
    {
    other.m_texture_handle = std::nullopt;
}

Font::Font(std::string font_name, u32 start_codepoint, u32 end_codepoint, u32 pixel_height) :
        m_start_codepoint(start_codepoint),
        m_end_codepoint(end_codepoint),
        m_pixel_height(pixel_height){
    std::string full_path = global_data_path + "font/" + font_name;
    if(std::filesystem::exists(full_path)) {

        //std::vector<char> buffer;
        std::ifstream file;
        file.open(full_path, std::ios::binary);
        file.seekg (0, file.end);
        u32 length = file.tellg();
        file.seekg (0, file.beg);
        m_font_data.resize(length);
        file.read(reinterpret_cast<char*>(m_font_data.data()), m_font_data.size());
        file.close();

        stbtt_InitFont(&m_font_info, m_font_data.data(), 0);
        m_scale = stbtt_ScaleForPixelHeight(&m_font_info, pixel_height);
        int x0, y0, x1, y1;
        stbtt_GetFontBoundingBox(&m_font_info, &x0, &y0, &x1, &y1);
        m_bounding_box = {x0, y0, x1, y1};
        int ascent, descent, lineGap;
        if(stbtt_GetFontVMetricsOS2(&m_font_info, &ascent, &descent, &lineGap) == 0) {
            stbtt_GetFontVMetrics(&m_font_info, &ascent, &descent, &lineGap);
        }
        m_baseline = m_scale*-y0;
        //m_bounding_box_size = {(abs(x1) + abs(x0)) * m_scale, (abs(y1) + abs(y0)) * m_scale};

        //m_bitmap_size.x = pixel_height;
        m_bitmap_size.x = (f32)(abs(m_bounding_box.x) + abs(m_bounding_box.z)) * m_scale;
        m_bitmap_size.y = (f32)(abs(ascent) + abs(descent)) * m_scale;
        //m_bitmap_size.y = (abs(ascent) + abs(descent)) * m_scale;

        const auto core_count = std::thread::hardware_concurrency();

        struct Workload {
            std::vector<std::vector<unsigned char>> bitmaps;
            u32 codepoint = 0;
        };

        std::vector<Workload> workloads;
        std::vector<std::thread> threads;
        workloads.resize(core_count);
        threads.resize(core_count-1);

        u32 base_count = (m_end_codepoint-m_start_codepoint) / core_count;
        u32 rem = ((m_end_codepoint-m_start_codepoint) % core_count);
        u32 cur_codepoint = m_start_codepoint;
        for(auto& workload : workloads) {
            workload.bitmaps = std::vector<std::vector<unsigned char>>(base_count + 1, std::vector<unsigned char>(m_bitmap_size.x * m_bitmap_size.y * 3));
            workload.codepoint = cur_codepoint;
            if(rem > 0) {
                rem--;
            } else {
                workload.bitmaps.resize(base_count);
            }
            cur_codepoint += workload.bitmaps.size();
        }

        int i = 1;
        for(auto& thread : threads) {
            thread = std::thread([](Workload* workload, Font* font) {
                for(auto& bitmap : workload->bitmaps) {
                    bitmap = font->generateBitmap(workload->codepoint);
                    workload->codepoint++;
                }
            }, &workloads[i], this);
            i++;
        }

        for(auto& bitmap : workloads[0].bitmaps) {
            bitmap = generateBitmap(workloads[0].codepoint);
            workloads[0].codepoint++;
        }

        for(auto& thread : threads) {
            thread.join();
        }

        for(int i = getStartCodepoint(); i < getEndCodepoint() + 1; ++i) {
            int index = stbtt_FindGlyphIndex(&m_font_info, i);
            if(index) {
                Glyph glyph;
                glyph.codepoint = (u32)i;
                glyph.index = (u32)index;

                stbtt_GetGlyphBox(&m_font_info, glyph.index, &glyph.bounding_box.x0, &glyph.bounding_box.y0, &glyph.bounding_box.x1, &glyph.bounding_box.y1);

                glyph.uv.top_left = {0.5 - (m_bitmap_size.x / ((f32)(abs(glyph.bounding_box.x0) + abs(glyph.bounding_box.x1)) / 2.0f)),
                                     0.5 + ( m_bitmap_size.y / ((f32)(abs(glyph.bounding_box.y0) + abs(glyph.bounding_box.y1)) / 2.0f))};
                glyph.uv.top_right = {0.5 + (m_bitmap_size.x / ((f32)(abs(glyph.bounding_box.x0) + abs(glyph.bounding_box.x1)) / 2.0f)),
                                      0.5 + ( m_bitmap_size.y / ((f32)(abs(glyph.bounding_box.y0) + abs(glyph.bounding_box.y1)) / 2.0f))};
                glyph.uv.bottom_left = {0.5 - (m_bitmap_size.x / ((f32)(abs(glyph.bounding_box.x0) + abs(glyph.bounding_box.x1)) / 2.0f)),
                                        0.5 - ( m_bitmap_size.y / ((f32)(abs(glyph.bounding_box.y0) + abs(glyph.bounding_box.y1)) / 2.0f))};
                glyph.uv.bottom_right = {0.5 + (m_bitmap_size.x / ((f32)(abs(glyph.bounding_box.x0) + abs(glyph.bounding_box.x1)) / 2.0f)),
                                         0.5 + (m_bitmap_size.y / ((f32)(abs(glyph.bounding_box.y0) + abs(glyph.bounding_box.y1)) / 2.0f))};

                m_glyphs.push_back({(u32)i, (u32)index});
            }
        }

        m_space_glyph.codepoint = ' ';
        m_space_glyph.index = stbtt_FindGlyphIndex(&m_font_info, ' ');

        getKernAdvance(24, 114, 114);

        // Generate array texture
        u32 texture;
        glGenTextures(1, &texture); CheckGLError();
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture); CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, m_bitmap_size.x, m_bitmap_size.y, m_end_codepoint - m_start_codepoint); CheckGLError();

        int depth = 0;
        for(auto& workload : workloads) {
            for(auto& bitmap : workload.bitmaps) {
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, depth, m_bitmap_size.x, m_bitmap_size.y, 1, GL_RGB, GL_UNSIGNED_BYTE, bitmap.data()); CheckGLError();
                depth++;
            }
        }

        m_texture_handle = texture; CheckGLError();

        stbi_write_bmp("test.bmp", m_bitmap_size.x, m_bitmap_size.y, 3, reinterpret_cast<void*>(workloads[6].bitmaps[1].data()));

    }
}

Font::~Font() {
    if(m_texture_handle.has_value()) {
        glDeleteTextures(1, &m_texture_handle.value());
    }
}

const std::optional<u32>& Font::getHandle() const {
    return m_texture_handle;
}

glm::vec2 Font::getBitmapSize() const {
    return m_bitmap_size;
}

glm::vec2 Font::getBoundingBoxSize() const {
    return { (f32)(abs(m_bounding_box.x) + abs(m_bounding_box.z)),  (f32)(abs(m_bounding_box.y) + abs(m_bounding_box.w))};
}

u32 Font::getPixelHeight() const {
    return m_pixel_height;
}

i32 Font::getKernAdvance(u32 pixel_height, u32 c0, u32 c1) const {
    std::optional<Glyph> c0g;
    std::optional<Glyph> c1g;
    if(c0 == (u32)' ' && c1 >= getStartCodepoint() && c1 <= getEndCodepoint()) {
        c0g = m_space_glyph;
        c1g = m_glyphs[c1 - getStartCodepoint()];
    } else if(c1 == (u32)' ' && c0 >= getStartCodepoint() && c0 <= getEndCodepoint()) {
        c0g = m_glyphs[c0 - getStartCodepoint()];
        c1g = m_space_glyph;
    } else if(c0 >= getStartCodepoint() && c0 <= getEndCodepoint() &&
       c1 >= getStartCodepoint() && c1 <= getEndCodepoint()) {
        c0g = m_glyphs[c0 - getStartCodepoint()];
        c1g = m_glyphs[c1 - getStartCodepoint()];
    }
    if(c0g.has_value() && c1g.has_value()) {
        int advance;
        advance = stbtt_GetGlyphKernAdvance(&m_font_info, c0g.value().index, c1g.value().index);
        return advance;
    }
    return 0;
}

void Font::getGlyphAdvance(u32 pixel_height, u32 glyph, i32& advance, i32& lsb) const {
    std::optional<Glyph> g;
    if(glyph == (u32)' ') {
        g = m_space_glyph;
    } else if(glyph >= getStartCodepoint() && glyph <= getEndCodepoint()) {
        g = m_glyphs[glyph - getStartCodepoint()];
    }
    if(g.has_value()) {
        stbtt_GetGlyphHMetrics(&m_font_info, g.value().index, &advance, &lsb);
    } else {
        advance = 0;
        lsb = 0;
    }
}

u32 Font::getStartCodepoint() const {
    return m_start_codepoint;
}

u32 Font::getEndCodepoint() const {
    return m_end_codepoint;
}

u32 Font::calcStringPixelWidth(std::string& str, u32 pixel_height) const {
    i32 xpos = 0;
    for (int i = 0; i < str.size(); ++i) {

        i32 advance;
        i32 lsb = 0;
        getGlyphAdvance(32, str[i], advance, lsb);

        // Calc an offset for the glyph to orient it in accordence to the font metrics since they always render from the center.
        int xoffset = advance / 2;
        if (i == 0) {
            xoffset -= lsb; // lsb is always negative
        }

        xpos += advance;
        int kern = 0;
        if (i < str.size() - 1) {
            xpos += getKernAdvance(32, str[i], str[i + 1]);
        }

    }
    return xpos * calcScale(pixel_height);
}

f32 Font::calcScale(u32 pixel_height) const {
    return m_scale * ((f32)pixel_height / (f32)m_pixel_height);
}

std::vector<unsigned char> Font::generateBitmap(u32 codepoint) {
    // Calculate position/scalers
    msdfgen::Vector2 position;
    int x0, y0, x1, y1;
    stbtt_GetCodepointBox(&m_font_info, codepoint, &x0, &y0, &x1, &y1);
    int ascent, descent, lineGap;
    if(stbtt_GetFontVMetricsOS2(&m_font_info, &ascent, &descent, &lineGap) == 0) {
        stbtt_GetFontVMetrics(&m_font_info, &ascent, &descent, &lineGap);
    }
    position.y = (m_baseline / m_scale);
    position.x = ((m_bitmap_size.x / m_scale) / 2) - ((abs(x0) + abs(x1)) / 2);

    // Build the MSDF shape
    stbtt_vertex* vertices = nullptr;
    int vertex_count = stbtt_GetCodepointShape(&m_font_info, codepoint, &vertices);
    msdfgen::Shape shape;
    msdfgen::Contour* cur_contour;
    msdfgen::Point2 last = {0.0, 0.0};
    msdfgen::Point2 cur = {0.0, 0.0};
    for(int i = 0; i < vertex_count; ++i) {
        if(vertices[i].type == STBTT_vmove) {
            cur_contour = &shape.addContour();
            cur_contour->edges.reserve(vertex_count);
            cur = { (double)(vertices[i].x), (double)(vertices[i].y)};
        } else if (vertices[i].type == STBTT_vline) {
            last = cur;
            cur = { (double)(vertices[i].x), (double)(vertices[i].y)};
            msdfgen::EdgeHolder new_edge(last, cur);
            cur_contour->addEdge(new_edge);
        } else if (vertices[i].type == STBTT_vcurve) {
            last = cur;
            cur = { (double)(vertices[i].x), (double)(vertices[i].y)};
            msdfgen::EdgeHolder new_edge(last, {(double)(vertices[i].cx), (double)(vertices[i].cy)}, cur);
            cur_contour->addEdge(new_edge);
        } else if (vertices[i].type == STBTT_vcubic) {
            last = cur;
            cur = { (double)(vertices[i].x), (double)(vertices[i].y  )};
            msdfgen::EdgeHolder new_edge(last, {(double)(vertices[i].cx), (double)(vertices[i].cy)}, {(double)(vertices[i].cx1), (double)(vertices[i].cy1)}, cur);
            cur_contour->addEdge(new_edge);
        }
    }
    stbtt_FreeShape(&m_font_info, vertices);
    shape.normalize();
    msdfgen::edgeColoringSimple(shape, 3.0);

    // Generate the bitmap
    msdfgen::Bitmap<float, 3> bitmap(m_bitmap_size.x, m_bitmap_size.y);
    msdfgen::generateMSDF(bitmap, shape, m_msdf_pixel_range/m_scale, m_scale, position);

    msdfgen::BitmapConstRef<float, 3> bitmapConst = bitmap;
    std::vector<unsigned char> pixels(3*bitmapConst.width*bitmapConst.height);
    std::vector<unsigned char>::iterator it = pixels.begin();
    for (int y = bitmapConst.height-1; y >= 0; --y) {
        for (int x = 0; x < bitmapConst.width; ++x) {
            *it++ = msdfgen::pixelFloatToByte(bitmapConst(x, y)[0]);
            *it++ = msdfgen::pixelFloatToByte(bitmapConst(x, y)[1]);
            *it++ = msdfgen::pixelFloatToByte(bitmapConst(x, y)[2]);
        }
    }

    return pixels;

}