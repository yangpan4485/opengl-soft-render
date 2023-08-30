#include "pixel_compiler_triangle.h"

#include <cmath>
#include <iostream>
#include "pipeline_utils.h"

PixelCompilerTriangle::PixelCompilerTriangle() = default;

PixelCompilerTriangle::~PixelCompilerTriangle() = default;

PixelAttri PixelCompilerTriangle::Decode(const Point& point) {
    PixelAttri result;
    result.x = point.point_position.x;
    result.y = point.point_position.y;
    result.z = Min(point.point_position.z * pow(2, 24), pow(2, 24) - 1);
    result.r = Min(point.point_attributes[0].attri[0] * 256, 255);
    result.g = Min(point.point_attributes[0].attri[1] * 256, 255);
    result.b = Min(point.point_attributes[0].attri[2] * 256, 255);
    result.a = 1.0 * 255;
    // std::cout << point.point_attributes[0].attri[0] << " "
    //           << point.point_attributes[0].attri[1] << " "
    //           << point.point_attributes[0].attri[2] << std::endl;
    return result;
}