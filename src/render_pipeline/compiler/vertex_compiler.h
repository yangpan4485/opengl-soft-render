#pragma once

#include <vector>
#include <cstdint>

#include "../pipeline_struct.h"

class VertexCompiler {
public:
    VertexCompiler();
    virtual ~VertexCompiler();

    virtual Point Decode(const std::vector<std::vector<float>>& attributes);
    virtual uint32_t AttributeCount();
};