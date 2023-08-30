#pragma once

#include "pixel_compiler.h"

class PixelCompilerColor : public PixelCompiler {
public:
    PixelCompilerColor();
    ~PixelCompilerColor();

    PixelAttri Decode(const Point& point) override;
};