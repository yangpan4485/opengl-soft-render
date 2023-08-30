#include "clip.h"

#include <iostream>

#include "pipeline_utils.h"

// Ax + By + Cz + D = 0;
const std::vector<std::vector<float>> view_lines{{0, 0, 1, 1}, { 0, 0, -1, 1 }, { 1,0,0,1 }, 
    { -1,0,0,1 }, { 0,-1,0,1 }, { 0,1,0,1 }};
// const std::vector<std::vector<float>> view_lines{{0, 0, 1, 1}, { 0, 0, -1, 1 }};

Clip::Clip() {}
Clip::~Clip() {}

void Clip::SetPrimitiveType(int type) {
    primitive_type_ = type;
    cullface_.SetPrimitiveType(type);
    primitive_point_count_ = GetPrimitivePointCount(type);
}

void Clip::SetAttributeNumber(uint32_t number) {
    arrtibute_number_ = number;
    cullface_.SetAttributeNumber(number);
}

void Clip::SetCullfaceConfig(const CullfaceConfig& config) {
    cullface_.SetCullfaceConfig(config);
}

void Clip::SetViewportConfig(const ViewportConfig& config) {
    cullface_.SetViewportConfig(config);
}

void Clip::SetRasterConfig(const RasterConfig& config) {
    cullface_.SetRasterConfig(config);
}

void Clip::PushData(const Point& point_data) {
    // std::cout << "clip: " << point_data.point_position.x << " " << point_data.point_position.y << std::endl;
    if (primitive_type_ == GL_POINTS) {
        cullface_.PushData(point_data);
        return;
    }
    point_attri_vec_.push_back(point_data);
    if (primitive_point_count_ == point_attri_vec_.size()) {
        if (primitive_point_count_ == 3) {
            ClipTriangle();
        } else {
            ClipLine();
        }
        point_attri_vec_.clear();
    }
}

bool Clip::InsideFace(const PointPosition& pos, const std::vector<float>& plane) {
    return plane[0] * pos.x + plane[1] * pos.y + plane[2] * pos.z + plane[3] * pos.w >= 0;
}

bool Clip::OutsideFace(const PointPosition& pos, const std::vector<float>& plane) {
    return !InsideFace(pos, plane);
}

Point Clip::Intersect(const Point& p1, const Point& p2, const std::vector<float>& plane) {
    float da = p1.point_position.x * plane[0] + p1.point_position.y * plane[1] + p1.point_position.z * plane[2] + p1.point_position.w * plane[3];
    float db = p2.point_position.x * plane[0] + p2.point_position.y * plane[1] + p2.point_position.z * plane[2] + p2.point_position.w * plane[3];
    float tmp = da / (da - db);
    Point output;
    output.point_position.x = p2.point_position.x * tmp + p1.point_position.x * (1 - tmp);
    output.point_position.y = p2.point_position.y * tmp + p1.point_position.y * (1 - tmp);
    output.point_position.z = p2.point_position.z * tmp + p1.point_position.z * (1 - tmp);
    output.point_position.w = p2.point_position.w * tmp + p1.point_position.w * (1 - tmp);
    output.point_size = p1.point_size;
    output.point_attributes.resize(arrtibute_number_ - 2);
    for (int i = 0; i < arrtibute_number_ - 2; ++i) {
        output.point_attributes[i] = p2.point_attributes[i] * tmp + p1.point_attributes[i] * (1 - tmp);
    }
    return output;
}

void Clip::ClipTriangle() {
    std::vector<Point> output = point_attri_vec_;
    for (const auto& type : view_lines) {
        std::vector<Point> input(output);
        if (input.size() < 3) {
            break;
        }
        output.clear();
        for (size_t i = 0; i < input.size(); ++i) {
            if (OutsideFace(input[i].point_position, type) && InsideFace(input[(i + 1) % input.size()].point_position, type)) {
                Point p = Intersect(input[i], input[(i + 1) % input.size()], type);
                output.push_back(p);
                continue;
            } else if (InsideFace(input[i].point_position, type) && OutsideFace(input[(i + 1) % input.size()].point_position, type)) {
                Point p = input[i];
                output.push_back(p);
                p = Intersect(input[i], input[(i + 1) % input.size()], type);
                output.push_back(p);
                continue;
            } else if (InsideFace(input[i].point_position, type) && InsideFace(input[(i + 1) % input.size()].point_position, type)) {
                Point p = input[i];
                output.push_back(p);
            }
        }
    }
    if (output.size() == 0) {
        return;
    }
    for (int i = 0; i < output.size() - 2; ++i) {
        for (size_t j = i; j < i + 3; ++j) {
            int index = j;
            if (j == i) {
                index = 0;
            }
            cullface_.PushData(output[index]);
        }
    }
}

void Clip::ClipLine() {
    std::vector<Point> output = point_attri_vec_;
    for (const auto& type : view_lines) {
        std::vector<Point> input(output);
        output.clear();
        if (OutsideFace(input[0].point_position, type) && InsideFace(input[1].point_position, type)) {
            Point p = Intersect(input[0], input[1], type);
            output.push_back(p);
            output.push_back(input[1]);
            break;
        } else if (InsideFace(input[0].point_position, type) && OutsideFace(input[1].point_position, type)) {
            output.push_back(input[0]);
            Point p = Intersect(input[0], input[1], type);
            output.push_back(p);
            break;
        } else if (InsideFace(input[0].point_position, type) && InsideFace(input[1].point_position, type)) {
            output.push_back(input[0]);
            output.push_back(input[1]);
        }
    }
    if (output.size() == 0) {
        return;
    }
    cullface_.PushData(output[0]);
    cullface_.PushData(output[1]);
}