#pragma once

#include "vertex_compiler.h"

class VertexCompilerTextureAdvanced : public VertexCompiler {
public:
    VertexCompilerTextureAdvanced();
    ~VertexCompilerTextureAdvanced();

    Point Decode(const std::vector<std::vector<float>>& attributes) override;
    uint32_t AttributeCount() override;
};