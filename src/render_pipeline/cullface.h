#pragma once

#include <vector>
#include <queue>
#include "pipeline_struct.h"
#include "viewport.h"

class Cullface {
public:
    Cullface();
    ~Cullface();

    void SetPrimitiveType(int type);
    void SetAttributeNumber(uint32_t number);
    void PushData(const Point& point_data);
    void CullfaceTriangle();
    void OutputData(bool front_face);
    void SetCullfaceConfig(const CullfaceConfig& config);
    void SetViewportConfig(const ViewportConfig& config);
    void SetRasterConfig(const RasterConfig& config);

private:
    int primitive_type_{};
    uint32_t arrtibute_number_{};
    // std::queue<float> attribute_data_queue_{};
    std::vector<Point> point_attri_vec_{};
    ViewPort viewport_;
    int arround_order_{GL_CCW};
    int cull_face_opt_{GL_BACK};
    bool enable_cullface_{};
};