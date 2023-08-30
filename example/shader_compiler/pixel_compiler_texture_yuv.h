#pragma once

#include "pixel_compiler.h"

class PixelCompilerTextureYUV : public PixelCompiler {
public:
    PixelCompilerTextureYUV();
    ~PixelCompilerTextureYUV();

    PixelAttri Decode(const Point& point) override;
};