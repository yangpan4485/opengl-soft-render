#pragma once
#include <memory>
#include "pipeline_struct.h"
#include "pixel_compiler.h"
#include "rop.h"

class PixelProcessor {
public:
    PixelProcessor();
    ~PixelProcessor();

    void PushData(const Point& point_data);

private:
    std::shared_ptr<PixelCompiler> pixel_compiler_{};
    std::shared_ptr<Rop> rop_ins_{};
};