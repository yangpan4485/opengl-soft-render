#pragma once

#include "vertex_compiler.h"

class VertexCompilerTexture : public VertexCompiler {
public:
    VertexCompilerTexture();
    ~VertexCompilerTexture();

    Point Decode(const std::vector<std::vector<float>>& attributes) override;
    uint32_t AttributeCount() override;
};