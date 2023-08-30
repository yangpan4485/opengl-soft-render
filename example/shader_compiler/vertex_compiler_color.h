#pragma once

#include "vertex_compiler.h"

class VertexCompilerColor : public VertexCompiler {
public:
    VertexCompilerColor();
    ~VertexCompilerColor();

    Point Decode(const std::vector<std::vector<float>>& attributes) override;
    uint32_t AttributeCount() override;
};