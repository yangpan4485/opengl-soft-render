#pragma once
#include "gl_common.h"
#include "shader_object.h"

struct ProgramObject {
    int program_name;
    std::unordered_map<std::string, int> uniform_location_map;
    std::unordered_map<int, std::string> attribute_location_map;
    std::unordered_map<std::string, int> custome_attribute_location_map;
    ShaderObject vertex_shader;
    ShaderObject fragment_shader;
};