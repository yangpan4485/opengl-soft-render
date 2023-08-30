#pragma once

#include "vertex_compiler.h"

class VertexCompilerCubeMap : public VertexCompiler {
public:
    VertexCompilerCubeMap();
    ~VertexCompilerCubeMap();

    Point Decode(const std::vector<std::vector<float>>& attributes) override;
    uint32_t AttributeCount() override;
};