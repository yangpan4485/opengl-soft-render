#pragma once

#include "vertex_compiler.h"

class VertexCompilerTriangle : public VertexCompiler {
public:
    VertexCompilerTriangle();
    ~VertexCompilerTriangle();

    Point Decode(const std::vector<std::vector<float>>& attributes) override;
    uint32_t AttributeCount() override;
};