#pragma once

#include "pixel_compiler.h"

class PixelCompilerTextureOverlay : public PixelCompiler {
public:
    PixelCompilerTextureOverlay();
    ~PixelCompilerTextureOverlay();

    PixelAttri Decode(const Point& point) override;
};