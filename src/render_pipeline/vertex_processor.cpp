#include "vertex_processor.h"

#include <iostream>

#include "compiler_manager.h"

VertexProcessor::VertexProcessor() {
}

VertexProcessor::~VertexProcessor() {
}

void VertexProcessor::PushData(const PointData& point_data) {
    vertex_compiler_ = CompilerManager::GetInstance().GetVertexCompiler();
    int first = point_data.first;
    int count = point_data.count;
    int primitive_type = (int)point_data.mode;
    primitive_assembly_.SetPrimitiveType(primitive_type);
    primitive_assembly_.SetAttributeNumber(vertex_compiler_->AttributeCount());
    int attribute_num = point_data.stride.size();
    for(int i = 0; i < count; ++i) {
        std::vector<std::vector<float>> attributes;
        for(int j = 0; j < attribute_num; ++j) {
            int stride = point_data.stride[j];
            std::vector<float> attri_data;
            for(int k = 0; k < stride; ++k) {
                // std::cout << j << " " << first * stride + k << std::endl;
                attri_data.push_back(point_data.data[j][first * stride + k]);
            }
            attributes.push_back(attri_data);
        }
        first = first + 1;
        Point result = vertex_compiler_->Decode(attributes);
        bool end = (i == count - 1);
        primitive_assembly_.PushData(result, end);
    }
}

void VertexProcessor::PushData(const PointData& point_data, const std::vector<int>& indices) {
    int attribute_num = point_data.stride.size();
    for (size_t i = 0; i < indices.size(); ++i) {
        int point_index = indices[0];
        std::vector<std::vector<float>> attributes;
        for (int j = 0; j < attribute_num; ++j) {
            std::vector<float> attri_data;
            int stride = point_data.stride[j];
            for(int k = 0; k < stride; ++k) {
                attri_data.push_back(point_data.data[j][stride * point_index + k]);
            }
            attributes.push_back(attri_data);
        }
        Point result = vertex_compiler_->Decode(attributes);
        bool end = (i == indices.size() - 1);
        primitive_assembly_.PushData(result, end);
    }
}

void VertexProcessor::SetCullfaceConfig(const CullfaceConfig& config) {
    primitive_assembly_.SetCullfaceConfig(config);
}

void VertexProcessor::SetViewportConfig(const ViewportConfig& config) {
    primitive_assembly_.SetViewportConfig(config);
}

void VertexProcessor::SetRasterConfig(const RasterConfig& config) {
    primitive_assembly_.SetRasterConfig(config);
}