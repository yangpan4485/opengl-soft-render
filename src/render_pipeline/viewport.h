#pragma once

#include <vector>
#include <queue>
#include "pipeline_struct.h"

#include "raster.h"

class ViewPort {
public:
    ViewPort();
    ~ViewPort();

    void SetPrimitiveType(int type);
    void SetAttributeNumber(uint32_t number);
    void SetViewportConfig(const ViewportConfig& config);
    void SetRasterConfig(const RasterConfig& config);
    void PushData(const Point& point_data);

private:
    int primitive_type_{};
    uint32_t arrtibute_number_{};
    Raster raster_{};

    int window_x_{};
    int window_y_{};
    int window_width_{};
    int window_height_{};
    float near_{};
    float far_{};
};