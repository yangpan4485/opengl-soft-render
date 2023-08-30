#pragma once
#include <cstdint>
#include <vector>
#include <queue>

#include "pipeline_struct.h"
#include "cullface.h"

class Clip {
public:
    Clip();
    ~Clip();

    void SetPrimitiveType(int type);
    void SetAttributeNumber(uint32_t number);
    void PushData(const Point& point_data);
    void ClipTriangle();
    void ClipLine();
    void SetCullfaceConfig(const CullfaceConfig& config);
    void SetViewportConfig(const ViewportConfig& config);
    void SetRasterConfig(const RasterConfig& config);

private:
    bool InsideFace(const PointPosition& pos, const std::vector<float>& plane);
    bool OutsideFace(const PointPosition& pos, const std::vector<float>& plane);
    Point Intersect(const Point& p1, const Point& p2, const std::vector<float>& plane);

private:
    int primitive_type_{};
    int primitive_point_count_{};
    uint32_t arrtibute_number_{};
    std::vector<Point> point_attri_vec_{};
    Cullface cullface_;
};