#include "texture_uncompress.h"

#include <iostream>

#include "texture_utils.h"

static const int lookup[8] = { 0, 1, 2, 3, -4, -3, -2, -1 };

TextureDecoder::TextureDecoder() {
    result_.resize(16);

    fout_.open("/home/yangpan/yangpan/test/etc_decoder/texture_output.txt", std::ios::binary | std::ios::out);
}

TextureDecoder::~TextureDecoder() {
    fout_.close();
}
void TextureDecoder::Decode(const std::vector<uint8_t>& data, uint32_t width, uint32_t height, int format, std::vector<uint8_t>& result_data) {
    result_.clear();
    uint8_t compress_data[8];
    uint8_t alpha_data[8];
    uint32_t size = 0;
    std::cout << std::hex << format << std::dec << std::endl;
    if (format == GL_COMPRESSED_RGB8_ETC2) {
        size = width * height / 2;
    } else if (format == GL_COMPRESSED_RGBA8_ETC2_EAC) {
        size = width * height;
    }
    int index_i = 0;
    int index_j = 0;
    for (int i = 0; i < size; ) {
        if (format == GL_COMPRESSED_RGBA8_ETC2_EAC) {
            for (int m = 0; m < 8; ++m) {
                alpha_data[m] = data[i + m];
            }
            i = i + 8;
        }
        for (int m = 0; m < 8; ++m) {
            compress_data[m] = data[i + m];
        }
        i = i + 8;
        DecompressData(format, compress_data, alpha_data);
        int index = 0;
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                int result_index = (index_j + k) * width + index_i + j;
                result_index = result_index * 4;
                result_data[result_index] = result_[index].r;
                result_data[result_index + 1] = result_[index].g;
                result_data[result_index + 2] = result_[index].b;
                result_data[result_index + 3] = result_[index].a;
                index++;
            }
        }
        index_i = index_i + 4;
        if (index_i == width) {
            index_i = 0;
            index_j = index_j + 4;
        }
    }
}

void TextureDecoder::DecompressData(int compress_type, uint8_t* compress_data, uint8_t* alpha_data) {
    if (compress_type == GL_COMPRESSED_RGB8_ETC2 || compress_type == GL_COMPRESSED_RGBA8_ETC2_EAC) {
        bool diffbit = false;
        uint8_t r, g, b, diff;
        r = compress_data[0];
        g = compress_data[1];
        b = compress_data[2];
        diff = compress_data[3];
        const int r_plus = (r >> 3) + lookup[r & 0x7];
        const int g_plus = (g >> 3) + lookup[g & 0x7];
        const int b_plus = (b >> 3) + lookup[b & 0x7];
        diffbit = diff & 0x2;
        if (!diffbit) {
            IndividualModeDecode(compress_data);
        }
        else if (r_plus < 0 || r_plus > 31) {
            TModeDecode(compress_data);
        }
        else if (g_plus < 0 || g_plus > 31) {
            HModeDecode(compress_data);
        }
        else if (b_plus < 0 || b_plus > 31) {
            PModeDecode(compress_data);
        }
        else if (diffbit) {
            DifferentialModeDecode(compress_data);
        }
    }
    if (compress_type == GL_COMPRESSED_RGBA8_ETC2_EAC) {
        AlphaModeDecode(alpha_data);
    }
}

void TextureDecoder::IndividualModeDecode(uint8_t* src) {
    uint8_t base_colors[3][3];
    for (int i = 0; i < 3; i++) {
        base_colors[0][i] = etc1_base_color_ind_hi(src[i]);
        base_colors[1][i] = etc1_base_color_ind_lo(src[i]);
    }
    bool flipped;
    int table1_idx = (src[3] >> 5) & 0x7;
    int table2_idx = (src[3] >> 2) & 0x7;
    const int* modifier_tables[2];
    modifier_tables[0] = etc1_modifier_tables[table1_idx];
    modifier_tables[1] = etc1_modifier_tables[table2_idx];
    flipped = (src[3] & 0x1);

    uint64_t pixel_indices[2];
    pixel_indices[0] = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];
    int index = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            int bit = y + x * 4;
            int idx = ((pixel_indices[0] >> (15 + bit)) & 0x2) | ((pixel_indices[0] >> (bit)) & 0x1);
            int blk = (flipped) ? (y >= 2) : (x >= 2);
            const uint8_t* base_color = base_colors[blk];
            int modifier = modifier_tables[blk][idx];
            result_[index].r = etc2_clamp(base_color[0] + modifier);
            result_[index].g = etc2_clamp(base_color[1] + modifier);
            result_[index].b = etc2_clamp(base_color[2] + modifier);
            index++;
        }
    }
}

void TextureDecoder::TModeDecode(uint8_t* src) {
    uint8_t base_colors[3][3];
    for (int i = 0; i < 3; i++) {
        base_colors[0][i] = etc2_base_color1_t_mode(src, i);
        base_colors[1][i] = etc2_base_color2_t_mode(src, i);
    }
    int distance = etc2_distance_table[(((src[3] >> 2) & 0x3) << 1) | (src[3] & 0x1)];
    uint8_t paint_colors[4][3];
    for (int i = 0; i < 3; i++) {
        paint_colors[0][i] = etc2_clamp(base_colors[0][i]);
        paint_colors[1][i] = etc2_clamp(base_colors[1][i] + distance);
        paint_colors[2][i] = etc2_clamp(base_colors[1][i]);
        paint_colors[3][i] = etc2_clamp(base_colors[1][i] - distance);
    }
    uint64_t pixel_indices[2];
    pixel_indices[0] = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];
    int index = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            int bit = y + x * 4;
            int idx = ((pixel_indices[0] >> (15 + bit)) & 0x2) | ((pixel_indices[0] >> (bit)) & 0x1);
            result_[index].r = paint_colors[idx][0];
            result_[index].g = paint_colors[idx][1];
            result_[index].b = paint_colors[idx][2];
            index++;
        }
    }
}

void TextureDecoder::HModeDecode(uint8_t* src) {
    int base_color_1_value, base_color_2_value;
    /* H mode */
    uint8_t base_colors[3][3];
    for (int i = 0; i < 3; i++) {
        base_colors[0][i] = etc2_base_color1_h_mode(src, i);
        base_colors[1][i] = etc2_base_color2_h_mode(src, i);
    }
    base_color_1_value = (base_colors[0][0] << 16) + (base_colors[0][1] << 8) + base_colors[0][2];
    base_color_2_value = (base_colors[1][0] << 16) + (base_colors[1][1] << 8) + base_colors[1][2];
    /* pick distance */
    int distance = etc2_distance_table[(src[3] & 0x4) | ((src[3] & 0x1) << 1) | (base_color_1_value >= base_color_2_value)];
    uint8_t paint_colors[4][3];
    for (int i = 0; i < 3; i++) {
        paint_colors[0][i] = etc2_clamp(base_colors[0][i] + distance);
        paint_colors[1][i] = etc2_clamp(base_colors[0][i] - distance);
        paint_colors[2][i] = etc2_clamp(base_colors[1][i] + distance);
        paint_colors[3][i] = etc2_clamp(base_colors[1][i] - distance);
    }
    uint64_t pixel_indices[2];
    pixel_indices[0] = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];
    int index = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            int bit = y + x * 4;
            int idx = ((pixel_indices[0] >> (15 + bit)) & 0x2) | ((pixel_indices[0] >> (bit)) & 0x1);
            result_[index].r = paint_colors[idx][0];
            result_[index].g = paint_colors[idx][1];
            result_[index].b = paint_colors[idx][2];
            index++;
        }
    }
}

void TextureDecoder::PModeDecode(uint8_t* src) {
    bool opaque = true;
    uint8_t base_colors[3][3];
    for (int i = 0; i < 3; i++) {
        base_colors[0][i] = etc2_base_color_o_planar(src, i);
        base_colors[1][i] = etc2_base_color_h_planar(src, i);
        base_colors[2][i] = etc2_base_color_v_planar(src, i);
    }
    uint64_t pixel_indices[2];
    pixel_indices[0] = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];
    int index = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            int bit = y + x * 4;
            int idx = ((pixel_indices[0] >> (15 + bit)) & 0x2) | ((pixel_indices[0] >> (bit)) & 0x1);
            int red, green, blue;
            red = (x * (base_colors[1][0] - base_colors[0][0]) +
                y * (base_colors[2][0] - base_colors[0][0]) +
                4 * base_colors[0][0] + 2) >> 2;

            green = (x * (base_colors[1][1] - base_colors[0][1]) +
                y * (base_colors[2][1] - base_colors[0][1]) +
                4 * base_colors[0][1] + 2) >> 2;

            blue = (x * (base_colors[1][2] - base_colors[0][2]) +
                y * (base_colors[2][2] - base_colors[0][2]) +
                4 * base_colors[0][2] + 2) >> 2;

            result_[index].r = etc2_clamp(red);
            result_[index].g = etc2_clamp(green);
            result_[index].b = etc2_clamp(blue);
            index++;
        }
    }
}

void TextureDecoder::DifferentialModeDecode(uint8_t* src) {
    uint8_t base_colors[3][3];
    for (int i = 0; i < 3; i++) {
        base_colors[0][i] = etc1_base_color_diff_hi(src[i]);
        base_colors[1][i] = etc1_base_color_diff_lo(src[i]);
    }
    int table1_idx = (src[3] >> 5) & 0x7;
    int table2_idx = (src[3] >> 2) & 0x7;
    const int* modifier_tables[2];
    modifier_tables[0] = etc1_modifier_tables[table1_idx];
    modifier_tables[1] = etc1_modifier_tables[table2_idx];
    bool  flipped = (src[3] & 0x1);

    uint64_t pixel_indices[2];
    pixel_indices[0] = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];
    int index = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {

            int bit = y + x * 4;
            int idx = ((pixel_indices[0] >> (15 + bit)) & 0x2) | ((pixel_indices[0] >> (bit)) & 0x1);
            int blk = (flipped) ? (y >= 2) : (x >= 2);
            const uint8_t* base_color = base_colors[blk];
            int modifier = modifier_tables[blk][idx];
            result_[index].r = etc2_clamp(base_color[0] + modifier);
            result_[index].g = etc2_clamp(base_color[1] + modifier);
            result_[index].b = etc2_clamp(base_color[2] + modifier);
            index++;
        }
    }
}

void TextureDecoder::AlphaModeDecode(uint8_t* src) {
    uint8_t base_codeword = src[0];
    uint8_t multiplier = (src[1] >> 4) & 0xf;
    uint8_t table_index = src[1] & 0xf;
    uint64_t pixel_indices[2];
    pixel_indices[1] = (((uint64_t)src[2] << 40) |
                        ((uint64_t)src[3] << 32) |
                        ((uint64_t)src[4] << 24) |
                        ((uint64_t)src[5] << 16) |
                        ((uint64_t)src[6] << 8)  |
                        ((uint64_t)src[7]));
    int index = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            int bit = ((3 - y) + (3 - x) * 4) * 3;
            int idx = (pixel_indices[1] >> bit) & 0x7;
            int modifier = etc2_modifier_tables[table_index][idx];
            int a = base_codeword + modifier * multiplier;
            result_[index].a = etc2_clamp(a);
            index++;
        }
    }
}