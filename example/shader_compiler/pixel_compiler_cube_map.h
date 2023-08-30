#pragma once

#include "pixel_compiler.h"

class PixelCompilerCubeMap : public PixelCompiler {
public:
    PixelCompilerCubeMap();
    ~PixelCompilerCubeMap();

    PixelAttri Decode(const Point& point) override;
};