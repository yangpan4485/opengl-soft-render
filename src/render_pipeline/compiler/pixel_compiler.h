#pragma once
#include <string>
#include "../pipeline_struct.h"

class PixelCompiler {
public:
    PixelCompiler();
    virtual ~PixelCompiler();
    virtual PixelAttri Decode(const Point& point);
    Color GetColor(const std::string& name, float u, float v);
};