#include "pixel_compiler.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include "texture/texture_data.h"

#include "utils.h"

PixelCompiler::PixelCompiler() {
}

PixelCompiler::~PixelCompiler() {
}

PixelAttri PixelCompiler::Decode(const Point& point) {
    PixelAttri result;
    return result;
}

Color PixelCompiler::GetColor(const std::string& name, float u, float v) {
    return TextureData::GetInstance().GetData(name, u, v);
}