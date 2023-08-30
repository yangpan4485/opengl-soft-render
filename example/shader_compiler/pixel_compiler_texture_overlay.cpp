#include "pixel_compiler_texture_overlay.h"

#include "texture/texture_data.h"
#include <cmath>
#include <iostream>
#include "pipeline_utils.h"

PixelCompilerTextureOverlay::PixelCompilerTextureOverlay() {
}

PixelCompilerTextureOverlay::~PixelCompilerTextureOverlay() {
}

PixelAttri PixelCompilerTextureOverlay::Decode(const Point& point)  {
    PixelAttri result;
    result.x = point.point_position.x;
    result.y = point.point_position.y;
    result.z = Min(point.point_position.z * pow(2, 24), pow(2, 24) - 1);
    float u = point.point_attributes[0].attri[0];
    float v = point.point_attributes[0].attri[1];
    Color color = TextureData::GetInstance().GetData("vTexture", u, v);
    Color overlay_color = TextureData::GetInstance().GetData("vOverlayTexture", u, v);
    // x * (1 - a) + y * a

    result.r = color.r * (1 - 0.1) + overlay_color.r * 0.1;
    result.g = color.g * (1 - 0.1) + overlay_color.g * 0.1;
    result.b = color.b * (1 - 0.1) + overlay_color.b * 0.1;
    result.a = color.a * (1 - 0.1) + overlay_color.a * 0.1;
    // std::cout << (int)result.r << " " << (int)result.g << " " << (int)result.b << " " << (int)result.a << std::endl;
    return result;
}