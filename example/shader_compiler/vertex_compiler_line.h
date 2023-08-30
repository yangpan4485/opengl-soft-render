#pragma once

#include "vertex_compiler.h"

class VertexCompilerLine : public VertexCompiler {
public:
    VertexCompilerLine();
    ~VertexCompilerLine();

    Point Decode(const std::vector<std::vector<float>>& attributes) override;
    uint32_t AttributeCount() override;
};