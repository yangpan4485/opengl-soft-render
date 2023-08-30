#include "pixel_compiler_color.h"

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "texture/texture_data.h"
#include "pipeline_utils.h"

PixelCompilerColor::PixelCompilerColor() {

}

PixelCompilerColor::~PixelCompilerColor() {

}

PixelAttri PixelCompilerColor::Decode(const Point& point) {
    PixelAttri result;
    result.x = point.point_position.x;
    result.y = point.point_position.y;
    result.z = Min(point.point_position.z * pow(2, 24), pow(2, 24) - 1);
    result.r = 0.04 * 255;
    result.g = 0.28 * 255;
    result.b = 0.26 * 255;
    result.a = 255;
    return result;
}