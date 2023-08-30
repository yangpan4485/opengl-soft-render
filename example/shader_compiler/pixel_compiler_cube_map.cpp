#include "pixel_compiler_cube_map.h"

#include "texture/texture_data.h"
#include <cmath>
#include <iostream>
#include "pipeline_utils.h"

PixelCompilerCubeMap::PixelCompilerCubeMap() {
}

PixelCompilerCubeMap::~PixelCompilerCubeMap() {
}

PixelAttri PixelCompilerCubeMap::Decode(const Point& point)  {
    PixelAttri result;
    result.x = point.point_position.x;
    result.y = point.point_position.y;
    result.z = Min(point.point_position.z * pow(2, 24), pow(2, 24) - 1);
    float x = point.point_attributes[0].attri[0];
    float y = point.point_attributes[0].attri[1];
    float z = point.point_attributes[0].attri[2];
    Color color = TextureData::GetInstance().GetCubeData("skybox", x, y, z);
    result.r = color.r;
    result.g = color.g;
    result.b = color.b;
    result.a = color.a;
    return result;
}