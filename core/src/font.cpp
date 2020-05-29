//
// Created by Feed on 5/27/2020.
//

#include "font.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"
#include "stb/stb_image_write.h"
#include "msdfgen/msdfgen.h"

#include <fstream>
#include <filesystem>
#include <iostream>

Font::Font() {}

Font::Font(std::string font_name, u32 w, u32 h, u32 pixel_height) : m_w(w), m_h(h), m_pixel_height(pixel_height) {
    std::string full_path = "data/" + font_name;
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
        stbtt_fontinfo font_info;
        stbtt_InitFont(&font_info, reinterpret_cast<unsigned char*>(buffer.data()), 0);
        m_scale = stbtt_ScaleForPixelHeight(&font_info, pixel_height);
        int x0, y0, x1, y1;
        stbtt_GetFontBoundingBox(&font_info, &x0, &y0, &x1, &y1);
        m_baseline = m_scale*-y0;

        auto bitmap = generateBitmap(font_info, 103);

        stbi_write_bmp("test.bmp", w, h, 3, reinterpret_cast<void*>(bitmap.data()));
    }
}

Font::~Font() {

}

b32 Font::isCompiled() {
    return m_compiled;
}

b32 Font::getGlyph(u32 index, Glyph& glyph) {
    return false;
}

const std::optional<u32>& Font::getTextureHandle() {
    return m_texture_handle;
}

f32 Font::getScale() {
    return m_scale;
}

glm::vec2 Font::getBitmapSize() {
    return { m_w, m_h };
}

std::vector<unsigned char> Font::generateBitmap(stbtt_fontinfo& font_info, u32 codepoint) {
    // Calculate position/scalers
    msdfgen::Vector2 position;
    int x0, y0, x1, y1;
    stbtt_GetCodepointBox(&font_info, codepoint, &x0, &y0, &x1, &y1);
    int ascent, descent, lineGap;
    if(stbtt_GetFontVMetricsOS2(&font_info, &ascent, &descent, &lineGap) == 0) {
        stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &lineGap);
    }
    position.y = m_baseline - (descent * m_scale); // Start with baseline
    position.x = (m_w/2) - ((abs(x0 * m_scale) + abs(x1 * m_scale))/2);

    // Build the MSDF shape
    stbtt_vertex* vertices = nullptr;
    int vertex_count = stbtt_GetCodepointShape(&font_info, codepoint, &vertices);
    std::cout << vertex_count << std::endl;
    msdfgen::Shape shape;
    msdfgen::Contour* cur_contour;
    msdfgen::Point2 last = {0.0, 0.0};
    msdfgen::Point2 cur = {0.0, 0.0};
    for(int i = 0; i < vertex_count; ++i) {
        if(vertices[i].type == STBTT_vmove) {
            cur_contour = &shape.addContour();
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
    stbtt_FreeShape(&font_info, vertices);
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