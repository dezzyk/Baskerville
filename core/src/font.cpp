//
// Created by Feed on 5/27/2020.
//

#include "font.h"
#include "platform.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"
#include "stb/stb_image_write.h"
#include "msdfgen/msdfgen.h"
#include "glad/glad.h"
#include "flatbuffers/generated/font_cache_serialize.h"

#include "gl_err.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <thread>

const Font* Font::Cache::load(std::string font_name,
                                std::string filename,
                                u32 pixel_height) {
    if(m_font_cache.find(font_name) == m_font_cache.end()) {
        m_font_cache.emplace(font_name, Font(font_name, filename, pixel_height));
    }
    return fetch(font_name);
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
        m_pixel_height(other.m_pixel_height),
        m_msdf_pixel_range(other.m_msdf_pixel_range),
        m_glyphs(std::move(other.m_glyphs)),
        m_codepoints(std::move(other.m_codepoints)),
        m_scale(other.m_scale)
    {
    other.m_texture_handle = std::nullopt;
}

Font::Font(std::string font_name, std::string filename, u32 pixel_height) : m_pixel_height(pixel_height) {
    if(!loadFromCache(font_name)) {
        loadFromFile(font_name, filename);
    }
}

b32 Font::loadFromCache(std::string font_name) {
    std::string cached_font_path = Platform::getPrefPath().string() + "font/";
    if(!std::filesystem::exists(cached_font_path)) {
        std::filesystem::create_directory(cached_font_path);
    }
    cached_font_path += font_name + ".dat";
    if(std::filesystem::exists(cached_font_path)) {

        // Load cache file, parse the values
        std::vector<u8> buffer;
        std::ifstream input;
        input.open(cached_font_path, std::ios::binary);
        input.seekg(0, input.end);
        u32 length = input.tellg();
        input.seekg(0, input.beg);
        buffer.resize(length);
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        input.close();

        auto font_cache = FontCacheSerialize::GetFontCache(buffer.data());
        auto data_from_cache = font_cache->data();
        auto cached_glyphs = font_cache->glyphs();
        m_font_data.assign(data_from_cache->begin(), data_from_cache->end());
        stbtt_InitFont(&m_font_info, m_font_data.data(), 0);

        // Load base metrics
        m_scale = stbtt_ScaleForPixelHeight(&m_font_info, m_pixel_height);
        int x0, y0, x1, y1;
        stbtt_GetFontBoundingBox(&m_font_info, &x0, &y0, &x1, &y1);
        m_bounding_box = {x0, y0, x1, y1};
        int ascent, descent, lineGap;
        if(stbtt_GetFontVMetricsOS2(&m_font_info, &ascent, &descent, &lineGap) == 0) {
            stbtt_GetFontVMetrics(&m_font_info, &ascent, &descent, &lineGap);
        }
        m_baseline = m_scale*-y0;

        m_bitmap_size = {font_cache->bitmap_width(), font_cache->bitmap_height()};

        // Load glyphs from font, followed by bitmaps from cache
        m_glyphs.resize(m_font_info.numGlyphs);

        u32 total_layers = 0;
        for(int i = 0; i < m_glyphs.size(); ++i) {
            m_glyphs[i].index = i;
            stbtt_GetGlyphBox(&m_font_info, m_glyphs[i].index, &m_glyphs[i].bounding_box.x0,
                    &m_glyphs[i].bounding_box.y0, &m_glyphs[i].bounding_box.x1, &m_glyphs[i].bounding_box.y1);
            if(!stbtt_IsGlyphEmpty(&m_font_info, m_glyphs[i].index)) {
                ++total_layers;
            }
        }

        for (int i = 0; i < m_codepoints.size(); ++i) {
            m_codepoints[i] = stbtt_FindGlyphIndex(&m_font_info, i + 1);
        }

        u32 texture;
        glGenTextures(1, &texture);
        CheckGLError();
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
        CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CheckGLError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, m_bitmap_size.x, m_bitmap_size.y, total_layers);
        CheckGLError();

        for(int i = 0; i < cached_glyphs->size(); ++i) {
            auto cached_glyph = cached_glyphs->Get(i);
            m_glyphs[cached_glyph->index()].layer = i;
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, m_bitmap_size.x, m_bitmap_size.y, 1,
                            GL_RGB, GL_UNSIGNED_BYTE, cached_glyph->bitmap()->data());
        }

        m_texture_handle = texture;
        return true;
    }
    return false;
}

b32 Font::loadFromFile(std::string font_name, std::string filename) {

    std::string raw_font_path = Platform::getDataPath() + "font/" + filename;
    if(std::filesystem::exists(raw_font_path)) {

        std::string cached_font_path = Platform::getPrefPath().string() + "font/" + font_name + ".dat";

        // Load font file
        std::ifstream file;
        file.open(raw_font_path, std::ios::binary);
        file.seekg(0, file.end);
        u32 length = file.tellg();
        file.seekg(0, file.beg);
        m_font_data.resize(length);
        file.read(reinterpret_cast<char *>(m_font_data.data()), m_font_data.size());
        file.close();

        stbtt_InitFont(&m_font_info, m_font_data.data(), 0);

        // Load base metrics
        m_scale = stbtt_ScaleForPixelHeight(&m_font_info, m_pixel_height);
        int x0, y0, x1, y1;
        stbtt_GetFontBoundingBox(&m_font_info, &x0, &y0, &x1, &y1);
        m_bounding_box = {x0, y0, x1, y1};
        int ascent, descent, lineGap;
        if(stbtt_GetFontVMetricsOS2(&m_font_info, &ascent, &descent, &lineGap) == 0) {
            stbtt_GetFontVMetrics(&m_font_info, &ascent, &descent, &lineGap);
        }
        m_baseline = m_scale*-y0;

        m_bitmap_size.x = (f32)(abs(m_bounding_box.x) + abs(m_bounding_box.z)) * m_scale;
        m_bitmap_size.y = (f32)(abs(ascent) + abs(descent)) * m_scale;

        // JOHN FUCKING MADDEN
        const auto core_count = std::thread::hardware_concurrency();

        if (m_font_info.numGlyphs > core_count) {

            struct PendingGlyph {
                std::vector<unsigned char> bitmap;
                b32 empty = false;
                int index = -1;
            };

            struct Workload {
                std::vector<PendingGlyph> glyphs;
                u32 index = 0;
                u32 drawable_glyph_count = 0;
            };

            std::vector<Workload> workloads;
            std::vector<std::thread> threads;
            workloads.resize(core_count);
            threads.resize(core_count - 1);

            u32 base_count = m_font_info.numGlyphs / core_count;
            u32 rem = (m_font_info.numGlyphs % core_count);
            u32 cur_index = 0;
            for (auto &workload : workloads) {
                workload.glyphs = std::vector<PendingGlyph>(base_count + 1, {
                        std::vector<unsigned char>(m_bitmap_size.x * m_bitmap_size.y * 3),
                        false,
                        -1
                });
                workload.index = cur_index;
                if (rem > 0) {
                    rem--;
                } else {
                    workload.glyphs.resize(base_count);
                }
                cur_index += workload.glyphs.size();
            }

            int i = 1;
            for (auto &thread : threads) {
                thread = std::thread([](Workload *workload, Font *font) {
                    for (auto &glyph : workload->glyphs) {
                        glyph.index = workload->index;
                        glyph.empty = !font->generateBitmap(glyph.index, glyph.bitmap);
                        if (!glyph.empty) {
                            workload->drawable_glyph_count++;
                        }
                        workload->index++;
                    }
                }, &workloads[i], this);
                i++;
            }

            for (auto &glyph : workloads[0].glyphs) {
                glyph.index = workloads[0].index;
                glyph.empty = !generateBitmap(glyph.index, glyph.bitmap);
                if (!glyph.empty) {
                    workloads[0].drawable_glyph_count++;
                }
                workloads[0].index++;
            }

            for (auto &thread : threads) {
                thread.join();
            }

            i32 total_layers = 0;
            for (auto &workload : workloads) {
                total_layers += workload.drawable_glyph_count;
            }

            // Generate array texture
            u32 texture;
            glGenTextures(1, &texture);
            CheckGLError();
            glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
            CheckGLError();
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            CheckGLError();
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            CheckGLError();
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, m_bitmap_size.x, m_bitmap_size.y, total_layers);
            CheckGLError();

            m_glyphs.resize(m_font_info.numGlyphs);

            // Build up the new cache while also building the array texture.
            flatbuffers::FlatBufferBuilder builder;
            std::vector<flatbuffers::Offset<FontCacheSerialize::Glyph>> serial_font_glyph_definitions;
            serial_font_glyph_definitions.resize(total_layers);

            int layer = 0;
            for (auto &workload : workloads) {
                for (auto &pending_glyph : workload.glyphs) {
                    Glyph &glyph = m_glyphs[pending_glyph.index];
                    if (!pending_glyph.empty) {
                        if (layer > total_layers) {
                            int i = 0;
                        }
                        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, m_bitmap_size.x, m_bitmap_size.y, 1,
                                        GL_RGB, GL_UNSIGNED_BYTE, pending_glyph.bitmap.data());
                        CheckGLError();
                        glyph.layer = layer;

                        auto new_serial_bitmap = builder.CreateVector(
                                reinterpret_cast<i8 *>(pending_glyph.bitmap.data()), pending_glyph.bitmap.size());
                        auto new_serial_glyph = FontCacheSerialize::CreateGlyph(builder, new_serial_bitmap, pending_glyph.index);
                        serial_font_glyph_definitions[layer] = new_serial_glyph;

                        ++layer;
                    }
                    glyph.index = pending_glyph.index;
                    stbtt_GetGlyphBox(&m_font_info, glyph.index, &glyph.bounding_box.x0, &glyph.bounding_box.y0,
                                      &glyph.bounding_box.x1, &glyph.bounding_box.y1);

                }
            }

            auto serial_font_data = builder.CreateVector(reinterpret_cast<i8 *>(m_font_data.data()),
                                                         m_font_data.size());
            auto serial_font_glyphs = builder.CreateVector(serial_font_glyph_definitions);
            auto finalized = FontCacheSerialize::CreateFontCache(builder, serial_font_data, serial_font_glyphs,
                                                                 m_bitmap_size.x, m_bitmap_size.y);

            builder.Finish(finalized);
            u8 *buf = builder.GetBufferPointer();
            int size = builder.GetSize();

            std::ofstream save_file(cached_font_path, std::ios::out | std::ios::binary);
            save_file.write(reinterpret_cast<char *>(buf), size);
            save_file.flush();

            m_texture_handle = texture;

            for (int i = 0; i < m_codepoints.size(); ++i) {
                m_codepoints[i] = stbtt_FindGlyphIndex(&m_font_info, i + 1);
            }

            return true;
        } else {

            // TODO: Set a required glyph count before multithreading.

        }
    }
    return false;
}

Font::~Font() {
    if(m_texture_handle.has_value()) {
        glDeleteTextures(1, &m_texture_handle.value());
    }
}

const std::optional<u32>& Font::getHandle() const {
    return m_texture_handle;
}

/*glm::vec2 Font::getBitmapSize() const {
    return m_bitmap_size;
}*/

glm::vec2 Font::getBoundingBoxSize() const {
    return { (f32)(abs(m_bounding_box.x) + abs(m_bounding_box.z)),  (f32)(abs(m_bounding_box.y) + abs(m_bounding_box.w))};
}

u32 Font::getPixelHeight() const {
    return m_pixel_height;
}

i32 Font::getKernAdvance(u32 pixel_height, u32 c0, u32 c1) const {
    if(c0 <= m_codepoints.size() && c1 <= m_codepoints.size()) {
        int advance;
        advance = stbtt_GetGlyphKernAdvance(&m_font_info,
                m_codepoints[c0], m_codepoints[c1]);
        return advance;
    }
    return 0;
}

void Font::getCodepointAdvance(u32 pixel_height, u32 codepoint, i32& advance, i32& lsb) const {
    if(codepoint <= m_codepoints.size()) {
        stbtt_GetGlyphHMetrics(&m_font_info, m_codepoints[codepoint-1], &advance, &lsb);
    } else {
        advance = 0;
        lsb = 0;
    }
}

u32 Font::calcStringPixelWidth(std::string& str, u32 pixel_height) const {
    i32 xpos = 0;
    for (int i = 0; i < str.size(); ++i) {

        i32 advance;
        i32 lsb = 0;
        getCodepointAdvance(pixel_height, str[i], advance, lsb);

        // Calc an offset for the glyph to orient it in accordence to the font metrics since they always render from the center.
        int xoffset = advance / 2;
        if (i == 0) {
            xoffset -= lsb; // lsb is always negative
        }

        xpos += advance;
        int kern = 0;
        if (i < str.size() - 1) {
            xpos += getKernAdvance(pixel_height, str[i], str[i + 1]);
        }

    }
    return xpos * calcScale(pixel_height);
}

i32 Font::getCodepointLayer(u32 codepoint) const {
    if(codepoint <= m_codepoints.size()) {
        if(m_codepoints[codepoint] != 0) {
            return m_glyphs[m_codepoints[codepoint-1]].layer;
        }
    }
    return -1;
}

i32 Font::getSpecificGlyphLayer(u32 index) const {
    if(index <= m_glyphs.size()) {
        return m_glyphs[index-1].layer;
    }
    return -1;
}

f32 Font::calcScale(u32 pixel_height) const {
    return m_scale * ((f32)pixel_height / (f32)m_pixel_height);
}

b32 Font::generateBitmap(u32 index, std::vector<unsigned char>& data) {
    if(stbtt_IsGlyphEmpty(&m_font_info, index)) {
        return false;
    }
    // Calculate position/scalers
    msdfgen::Vector2 position;
    int x0, y0, x1, y1;
    stbtt_GetGlyphBox(&m_font_info, index, &x0, &y0, &x1, &y1);
    int ascent, descent, lineGap;
    if(stbtt_GetFontVMetricsOS2(&m_font_info, &ascent, &descent, &lineGap) == 0) {
        stbtt_GetFontVMetrics(&m_font_info, &ascent, &descent, &lineGap);
    }
    position.y = (m_baseline / m_scale);
    position.x = ((m_bitmap_size.x / m_scale) / 2) - ((abs(x0) + abs(x1)) / 2);

    // Build the MSDF shape
    stbtt_vertex* vertices = nullptr;
    int vertex_count = stbtt_GetGlyphShape(&m_font_info, index, &vertices);
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
    std::vector<unsigned char>::iterator it = data.begin();
    for (int y = bitmapConst.height-1; y >= 0; --y) {
        for (int x = 0; x < bitmapConst.width; ++x) {
            *it++ = msdfgen::pixelFloatToByte(bitmapConst(x, y)[0]);
            *it++ = msdfgen::pixelFloatToByte(bitmapConst(x, y)[1]);
            *it++ = msdfgen::pixelFloatToByte(bitmapConst(x, y)[2]);
        }
    }

    return true;

}