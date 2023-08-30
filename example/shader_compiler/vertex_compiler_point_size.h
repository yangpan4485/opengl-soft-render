#pragma once

#include "vertex_compiler.h"

class VertexCompilerPointSize : public VertexCompiler {
public:
    VertexCompilerPointSize();
    ~VertexCompilerPointSize();

    Point Decode(const std::vector<std::vector<float>>& attributes) override;
    uint32_t AttributeCount() override;
};