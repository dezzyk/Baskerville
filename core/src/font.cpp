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

const Font* Font::Cache::load(std::string font_name, std::string filename, u32 w, u32 h, u32 start_codepoint, u32 end_codepoint, u32 pixel_height) {
    if(m_font_cache.find(font_name) == m_font_cache.end()) {
        m_font_cache.emplace(font_name, Font(filename, w, h, start_codepoint, end_codepoint, pixel_height));
    }
    return &m_font_cache[font_name];
}

const Font* Font::Cache::fetch(std::string font_name) {
    if(m_font_cache.find(font_name) == m_font_cache.end()) {
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
    m_texture_handle(other.m_texture_handle),
    m_scale(other.m_scale),
    m_baseline(other.m_baseline),
    m_w(other.m_w),
    m_h(other.m_h),
    m_pixel_height(other.m_pixel_height)
    {
    other.m_texture_handle = std::nullopt;
}

Font::Font(std::string font_name, u32 w, u32 h, u32 start_codepoint, u32 end_codepoint,  u32 pixel_height) :
                                                                                m_w(w),
                                                                                m_h(h),
                                                                                m_start_codepoint(start_codepoint),
                                                                                m_end_codepoint(end_codepoint),
                                                                                m_pixel_height(pixel_height) {
    std::string full_path = global_data_path + "font/" + font_name;
    if(std::filesystem::exists(full_path)) {

        std::vector<char> buffer;
        std::ifstream file;
        file.open(full_path, std::ios::binary);
        file.seekg (0, file.end);
        u32 length = file.tellg();
        file.seekg (0, file.beg);
        buffer.resize(length);
        file.read(buffer.data(), buffer.size());
        file.close();

        stbtt_InitFont(&m_font_info, reinterpret_cast<unsigned char*>(buffer.data()), 0);
        m_scale = stbtt_ScaleForPixelHeight(&m_font_info, pixel_height);
        int x0, y0, x1, y1;
        stbtt_GetFontBoundingBox(&m_font_info, &x0, &y0, &x1, &y1);
        m_baseline = m_scale*-y0;

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
            workload.bitmaps = std::vector<std::vector<unsigned char>>(base_count + 1, std::vector<unsigned char>(m_w * m_h * 3));
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

        // Generate array texture
        u32 texture;
        glGenTextures(1, &texture); CheckGLError();
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture); CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CheckGLError();
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, m_w, m_h, m_end_codepoint-m_start_codepoint); CheckGLError();

        int depth = 0;
        for(auto& workload : workloads) {
            for(auto& bitmap : workload.bitmaps) {
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, depth, m_w, m_h, 1, GL_RGB, GL_UNSIGNED_BYTE, bitmap.data()); CheckGLError();
                //std::cout << depth << std::endl;
                //std::cout << bitmap.size() << std::endl;
                depth++;
            }
        }

        m_texture_handle = texture; CheckGLError();

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

f32 Font::getScale() const{
    return m_scale;
}

f32 Font::getBaseline() const{
    return m_baseline;
}

glm::vec2 Font::getBitmapSize() const{
    return { m_w, m_h };
}

u32 Font::getKernOffset(u32 c0, u32 c1) const{
    return stbtt_GetCodepointKernAdvance(&m_font_info, c0, c1);
}

u32 Font::getStartCodepoint() const{
    return m_start_codepoint;
}

u32 Font::getEndCodepoint() const{
    return m_end_codepoint;
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
    position.y = m_baseline - (descent * m_scale); // Start with baseline
    position.x = (m_w/2) - ((abs(x0 * m_scale) + abs(x1 * m_scale))/2);

    // Build the MSDF shape
    stbtt_vertex* vertices = nullptr;
    int vertex_count = stbtt_GetCodepointShape(&m_font_info, codepoint, &vertices);
    //std::cout << vertex_count << std::endl;
    msdfgen::Shape shape;
    //msdfgen::Contour* cur_contour;
    msdfgen::Contour* cur_contour;
    msdfgen::Point2 last = {0.0, 0.0};
    msdfgen::Point2 cur = {0.0, 0.0};
    for(int i = 0; i < vertex_count; ++i) {
        if(vertices[i].type == STBTT_vmove) {
            cur_contour = &shape.addContour();
            cur_contour->edges.reserve(vertex_count); // ez optimization lul
            cur = { (double)(vertices[i].x * m_scale), (double)(vertices[i].y  * m_scale)};
        } else if (vertices[i].type == STBTT_vline) {
            last = cur;
            cur = { (double)(vertices[i].x * m_scale), (double)(vertices[i].y  * m_scale)};
            msdfgen::EdgeHolder new_edge(last, cur);
            cur_contour->addEdge(new_edge);
        } else if (vertices[i].type == STBTT_vcurve) {
            last = cur;
            cur = { (double)(vertices[i].x * m_scale), (double)(vertices[i].y  * m_scale)};
            msdfgen::EdgeHolder new_edge(last, {(double)(vertices[i].cx * m_scale), (double)(vertices[i].cy * m_scale)}, cur);
            cur_contour->addEdge(new_edge);
        } else if (vertices[i].type == STBTT_vcubic) {
            last = cur;
            cur = { (double)(vertices[i].x * m_scale), (double)(vertices[i].y  * m_scale)};
            msdfgen::EdgeHolder new_edge(last, {(double)(vertices[i].cx * m_scale), (double)(vertices[i].cy * m_scale)}, {(double)(vertices[i].cx1 * m_scale), (double)(vertices[i].cy1 * m_scale)}, cur);
            cur_contour->addEdge(new_edge);
        }
    }
    stbtt_FreeShape(&m_font_info, vertices);
    shape.normalize();
    msdfgen::edgeColoringSimple(shape, 3.0);

    // Generate the bitmap
    msdfgen::Bitmap<float, 3> bitmap(m_w, m_h);
    msdfgen::generateMSDF(bitmap, shape, 4.0, 1.0, position);

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