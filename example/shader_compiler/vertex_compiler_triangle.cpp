#include "vertex_compiler_triangle.h"

VertexCompilerTriangle::VertexCompilerTriangle() = default;

VertexCompilerTriangle::~VertexCompilerTriangle() = default;

Point VertexCompilerTriangle::Decode(const std::vector<std::vector<float>>& attributes) {
    Point point_attri;
    // gl_Position = vec4(vPosition, 1.0);
    point_attri.point_position.x = attributes[0][0];
    point_attri.point_position.y = attributes[0][1];
    point_attri.point_position.z = attributes[0][2];
    point_attri.point_position.w = 1.0f;
    // point size default 1.0
    point_attri.point_size = 1.0;
    PointAttribute attri;
    for (int i = 0; i < 3; ++i) {
        attri.attri[i] = attributes[1][i];
    }
    point_attri.point_attributes.push_back(attri);
    return point_attri;
}

uint32_t VertexCompilerTriangle::AttributeCount() {
    return 3;
}