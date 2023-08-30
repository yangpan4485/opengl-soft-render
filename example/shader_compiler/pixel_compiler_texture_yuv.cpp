#include "pixel_compiler_texture_yuv.h"

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "texture/texture_data.h"
#include "pipeline_utils.h"

PixelCompilerTextureYUV::PixelCompilerTextureYUV() {

}

PixelCompilerTextureYUV::~PixelCompilerTextureYUV() {

}

PixelAttri PixelCompilerTextureYUV::Decode(const Point& point) {
    PixelAttri result;
    glm::vec3 yuv;
    glm::vec3 rgb;
    float u = point.point_attributes[0].attri[0];
    float v = point.point_attributes[0].attri[1];
    
    // std::cout << u << " " << v << std::endl;
    Color y_color = TextureData::GetInstance().GetData("tex_y", u, v);
    Color u_color = TextureData::GetInstance().GetData("tex_u", u, v);
    Color v_color = TextureData::GetInstance().GetData("tex_v", u, v);
    // return result;
    yuv.x = y_color.r * 1.0 / 255;
    yuv.y = u_color.r * 1.0 / 255 - 0.5;
    yuv.z = v_color.r * 1.0 / 255- 0.5;

    rgb = glm::mat3(1,       1,        1,
                    0,       -0.39465, 2.03211,
                    1.13983, -0.58060, 0) * yuv;

    
    result.x = point.point_position.x;
    result.y = point.point_position.y;
    result.z = Min(point.point_position.z * pow(2, 24), pow(2, 24) - 1);
    result.r = rgb.r * 255;
    result.g = rgb.g * 255;
    result.b = rgb.b * 255;
    result.a = 255;
    return result;
}