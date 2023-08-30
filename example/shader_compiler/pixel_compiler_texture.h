#pragma once

#include "pixel_compiler.h"

class PixelCompilerTexture : public PixelCompiler {
public:
    PixelCompilerTexture();
    ~PixelCompilerTexture();

    PixelAttri Decode(const Point& point) override;
};