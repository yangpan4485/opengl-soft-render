#pragma once
#include <unordered_map>
#include <string>

#include "gl_common.h"

struct ShaderObject {
    // std::unordered_map<std::string, int> uniform_location_map;
    // std::unordered_map<int, std::string> attribute_location_map;
    // std::unordered_map<std::string, int> custome_attribute_location_map;
    int shader_type{};
    int shader_name{};
    bool is_attached{};
    bool need_delete{};
    std::string shader_source{};
};