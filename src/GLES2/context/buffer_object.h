#pragma once
#include <vector>
#include <unordered_map>

#include "gl_common.h"

struct VertexData {
    bool enable{};
    int index{};
    int stride{};
    int size{};
    float* pointer{};
    std::vector<float> data;
};

struct BufferObject {
    int buffer_type;
    int buffer_name;
    int usage;
    std::vector<float> buffer_data;
    std::unordered_map<int, VertexData> vertex_data_map;
};