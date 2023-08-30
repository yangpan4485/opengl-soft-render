#pragma once

#include "pixel_compiler.h"

class PixelCompilerTriangle : public PixelCompiler {
public:
    PixelCompilerTriangle();
    ~PixelCompilerTriangle();

    PixelAttri Decode(const Point& point) override;
};