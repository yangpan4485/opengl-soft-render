#pragma once

#include <vector>

#include "pipeline_struct.h"
#include "pa.h"
#include "vertex_compiler.h"
#include "context/buffer_object.h"
#include "context/gl_config.h"

class VertexProcessor {
public:
    VertexProcessor();
    ~VertexProcessor();
    void PushData(const PointData& point_data);
    void PushData(const PointData& point_data, const std::vector<int>& indices);

    void SetCullfaceConfig(const CullfaceConfig& config);
    void SetViewportConfig(const ViewportConfig& config);
    void SetRasterConfig(const RasterConfig& config);

private:
    PrimitiveAssembly primitive_assembly_;
    std::shared_ptr<VertexCompiler> vertex_compiler_;
};