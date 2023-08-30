#pragma once

#include <vector>
#include <cstdint>
#include <fstream>

#include "pipeline_struct.h"

static const int etc1_modifier_tables[8][4] = {
   {  2,   8,  -2,   -8},
   {  5,  17,  -5,  -17},
   {  9,  29,  -9,  -29},
   { 13,  42, -13,  -42},
   { 18,  60, -18,  -60},
   { 24,  80, -24,  -80},
   { 33, 106, -33, -106},
   { 47, 183, -47, -183}
};

static const int etc2_distance_table[8] = {3, 6, 11, 16, 23, 32, 41, 64 };

static const int etc2_modifier_tables[16][8] = {
   {  -3,   -6,   -9,  -15,   2,   5,   8,   14},
   {  -3,   -7,  -10,  -13,   2,   6,   9,   12},
   {  -2,   -5,   -8,  -13,   1,   4,   7,   12},
   {  -2,   -4,   -6,  -13,   1,   3,   5,   12},
   {  -3,   -6,   -8,  -12,   2,   5,   7,   11},
   {  -3,   -7,   -9,  -11,   2,   6,   8,   10},
   {  -4,   -7,   -8,  -11,   3,   6,   7,   10},
   {  -3,   -5,   -8,  -11,   2,   4,   7,   10},
   {  -2,   -6,   -8,  -10,   1,   5,   7,    9},
   {  -2,   -5,   -8,  -10,   1,   4,   7,    9},
   {  -2,   -4,   -8,  -10,   1,   3,   7,    9},
   {  -2,   -5,   -7,  -10,   1,   4,   6,    9},
   {  -3,   -4,   -7,  -10,   2,   3,   6,    9},
   {  -1,   -2,   -3,  -10,   0,   1,   2,    9},
   {  -4,   -6,   -8,   -9,   3,   5,   7,    8},
   {  -3,   -5,   -7,   -9,   2,   4,   6,    8},
};

class TextureDecoder {
public:
    TextureDecoder();
    ~TextureDecoder();
    void Decode(const std::vector<uint8_t>& data, uint32_t width, uint32_t height, int format, std::vector<uint8_t>& result_data);
    void DecompressData(int compress_type, uint8_t* compress_data, uint8_t* alpha_data);
    void IndividualModeDecode(uint8_t* src);
    void TModeDecode(uint8_t* src);
    void HModeDecode(uint8_t* src);
    void PModeDecode(uint8_t* src);
    void DifferentialModeDecode(uint8_t* src);
    void AlphaModeDecode(uint8_t* src);

private:
    std::vector<Color> result_{};
    std::ofstream fout_{};
};