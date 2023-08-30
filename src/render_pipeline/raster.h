#pragma once

#include <queue>
#include <vector>

#include "pipeline_struct.h"
#include "pixel_processor.h"

struct PointConfig {
    float coord_step{1.0};
};

struct LineConfig {
    int linewidth;
    float line_k;
    float line_b;
    bool y_increase;
};

struct TriangleConfig {
    bool need_handle_up{};
    bool need_handle_down{};
    Primitive up_triangle;
    Primitive down_triangle;
};

class Raster {
public:
    Raster();
    ~Raster();

    void SetPrimitiveType(int type);
    void SetAttributeNumber(uint32_t number);
    void SetRasterConfig(const RasterConfig& config);
    void SetViewportConfig(const ViewportConfig& config);
    void PushData(const Point& point_data);

private:
    void Swap(Point& a, Point& b);
    void DoRaster(const Point& left, const Point& right);
    void RasterPoint();
    void RasterLine();
    void RasterTriangleLinear();
    void RasterUpTriangleLinear();
    void RasterDownTriangleLinear();
    void RasterTriangleBarycentric();
    void RasterTriangleBarycentricBlock();

    bool CheckPoint(const Point& point);
    bool CheckPoint(const PointPosition& pos);

    // void HandlePoint();
    // void HandleLine();
    // void HandleTriangle();
    // void RasterTriangleBlock();
    // void Lerp(const Point& left, const Point& right);
    // Point Lerp(const Point& left, const Point& right, float weight, const Primitive& primitive_val);
    // PointPosition Lerp(const PointPosition& left, const PointPosition& right, float weight);
    // PointAttribute Lerp(const PointAttribute& left, const PointAttribute& right, float weight);

    // bool CheckLine(Point& left, Point& right);

    // void HandleUpTriangle();
    // void HandleDownTriangle();

private:
    int primitive_type_{};
    // Primitive primitive_val_{};
    uint32_t arrtibute_number_{};
    std::vector<Point> attribute_data_vec_{};

    // uint16_t window_x_{};
    // uint16_t window_y_{};
    // uint16_t window_width_{};
    // uint16_t window_height_{};
    PixelProcessor pixel_processor_{};

    PointConfig point_config_;
    LineConfig line_config_;
    TriangleConfig triangle_config_;

    // viewport
    int viewport_x_{};
    int viewport_y_{};
    int viewport_w_{};
    int viewport_h_{};
};