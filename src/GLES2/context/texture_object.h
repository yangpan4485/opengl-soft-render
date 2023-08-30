#pragma once
#include <unordered_map>
#include <string>

#include "gl_common.h"

struct TextureObject {
    int texture_name;
    int level;
    int internal_format;
    int width;
    int height;
    int format;
    int type;
    int pixel_storei{4};
    // int last_pixel_storei{4};
    // std::unordered_map<int, std::vector<uint8_t>> data_map;
    // std::unordered_map<int, std::vector<uint8_t>> cube_data_map;
    std::unordered_map<int, int> params_map;
};