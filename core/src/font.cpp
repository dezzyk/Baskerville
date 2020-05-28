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

Font::Font(std::string font_name) {
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
        //Testing with codepoint 65 (A)
        float scale = stbtt_ScaleForPixelHeight(&font_info, 64);
        stbtt_vertex* vertices = nullptr;
        int vertex_count = stbtt_GetCodepointShape(&font_info, 65, &vertices);
        std::cout << vertex_count << std::endl;
        msdfgen::Shape shape;
        msdfgen::Contour* cur_contour;
        msdfgen::Point2 last = {0.0, 0.0};
        msdfgen::Point2 cur = {0.0, 0.0};
        for(int i = 0; i < vertex_count; ++i) {
            if(vertices[i].type == STBTT_vmove) {
                cur_contour = &shape.addContour();
                cur = { (double)(vertices[i].x * scale), (double)(vertices[i].y  * scale)};
            } else if (vertices[i].type == STBTT_vline) {
                last = cur;
                cur = { (double)(vertices[i].x * scale), (double)(vertices[i].y  * scale)};
                msdfgen::EdgeHolder new_edge(last, cur);
                cur_contour->addEdge(new_edge);
            } else if (vertices[i].type == STBTT_vcurve) {
                last = cur;
                cur = { (double)(vertices[i].x * scale), (double)(vertices[i].y  * scale)};
                msdfgen::EdgeHolder new_edge(last, {(double)(vertices[i].cx * scale), (double)(vertices[i].cy * scale)}, cur);
                cur_contour->addEdge(new_edge);
            } else if (vertices[i].type == STBTT_vcubic) {
                last = cur;
                cur = { (double)(vertices[i].x * scale), (double)(vertices[i].y  * scale)};
                msdfgen::EdgeHolder new_edge(last, {(double)(vertices[i].cx * scale), (double)(vertices[i].cy * scale)}, {(double)(vertices[i].cx1 * scale), (double)(vertices[i].cy1 * scale)}, cur);
                cur_contour->addEdge(new_edge);
            }
        }
        stbtt_FreeShape(&font_info, vertices);
        shape.normalize();
        msdfgen::edgeColoringSimple(shape, 3.0);

        // Calc the translation based on the glyph metrics

        // Generate the MSDF
        msdfgen::Bitmap<float, 3> bitmap(64, 64);
        msdfgen::generateMSDF(bitmap, shape, 4.0, 1.0, msdfgen::Vector2(0.0, 0.0));

        //Temp saving just to have some tangible output
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
        stbi_write_bmp("test.bmp", 64, 64, 3, reinterpret_cast<void*>(pixels.data()));
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