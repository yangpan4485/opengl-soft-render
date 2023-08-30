#include "pixel_compiler_texture.h"

#include "texture/texture_data.h"
#include <cmath>
#include <iostream>
#include "pipeline_utils.h"

PixelCompilerTexture::PixelCompilerTexture() {
}

PixelCompilerTexture::~PixelCompilerTexture() {
}

PixelAttri PixelCompilerTexture::Decode(const Point& point)  {
    PixelAttri result;
    result.x = point.point_position.x;
    result.y = point.point_position.y;
    result.z = Min(point.point_position.z * pow(2, 24), pow(2, 24) - 1);
    float u = point.point_attributes[0].attri[0];
    float v = point.point_attributes[0].attri[1];
    Color color = TextureData::GetInstance().GetData("vTexture", u, v);
    // std::cout << u << " " << v << std::endl;
    // result.r = u * 255;
    // result.g = v * 255;
    // result.b = 0.5 * 255;
    // result.a = 1 * 255;
    // std::cout << "color.a: " << (int)color.a << std::endl;
    result.r = color.r;
    result.g = color.g;
    result.b = color.b;
    result.a = color.a;
    // result.a = 255;
    // std::cout << (int)result.r << " " << (int)result.g << " " << (int)result.b << " " << (int)result.a << std::endl;
    return result;
}