#pragma once

#include <vector>
#include <queue>
#include "pipeline_struct.h"

#include "clip.h"

class PrimitiveAssembly {
public:
    PrimitiveAssembly();
    ~PrimitiveAssembly();

    void SetPrimitiveType(int type);
    void SetAttributeNumber(uint32_t number);
    void PushData(const Point& point_data, bool is_end);

    void SetCullfaceConfig(const CullfaceConfig& config);
    void SetViewportConfig(const ViewportConfig& config);
    void SetRasterConfig(const RasterConfig& config);

private:
    void HandleLineData(bool is_end);
    void HandleTriangleData(bool is_end);

private:
    int primitive_type_{};
    uint32_t arrtibute_number_{};
    std::queue<Point> attribute_data_queue_{};

    Clip clip;
    bool line_loop_first_{true};
    Point line_loop_first_point_{};
    bool triangle_strip_flag_{};

};