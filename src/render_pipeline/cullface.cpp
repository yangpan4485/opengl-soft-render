#include "cullface.h"

#include <iostream>

Cullface::Cullface() {}

Cullface::~Cullface() {}

void Cullface::SetPrimitiveType(int type) {
    primitive_type_ = type;
    viewport_.SetPrimitiveType(type);
}

void Cullface::SetAttributeNumber(uint32_t number) {
    arrtibute_number_ = number;
    viewport_.SetAttributeNumber(arrtibute_number_ + 1);
}

void Cullface::SetCullfaceConfig(const CullfaceConfig& config) {
    arround_order_ = config.front_face;
    cull_face_opt_ = config.cullface_mode;
    enable_cullface_ = config.enable;
}

void Cullface::SetViewportConfig(const ViewportConfig& config) {
    viewport_.SetViewportConfig(config);
}

void Cullface::SetRasterConfig(const RasterConfig& config) {
    viewport_.SetRasterConfig(config);
}

void Cullface::PushData(const Point& point_data) {
    // std::cout << "cullface: " << point_data.point_position.x << " " << point_data.point_position.y << " " 
    //           << point_data.point_position.z << " " << point_data.point_position.w << " " 
    //           << point_data.point_attributes[0].attri[0] << " "
    //           << point_data.point_attributes[0].attri[1] << " "
    //           << point_data.point_attributes[0].attri[2] << std::endl;
    if (primitive_type_ < GL_TRIANGLES) {
        viewport_.PushData(point_data);
        return;
    }
    point_attri_vec_.push_back(point_data);
    if (point_attri_vec_.size() == 3) {
        CullfaceTriangle();
        point_attri_vec_.clear();
    }
}

void Cullface::CullfaceTriangle() {
    float a = point_attri_vec_[0].point_position.x * (point_attri_vec_[1].point_position.y - point_attri_vec_[2].point_position.y) +
              point_attri_vec_[1].point_position.x * (point_attri_vec_[2].point_position.y - point_attri_vec_[0].point_position.y) +
              point_attri_vec_[2].point_position.x * (point_attri_vec_[0].point_position.y - point_attri_vec_[1].point_position.y);
    bool front_face = false;
    if (arround_order_ == GL_CCW) {
        front_face = a > 0 ? true : false;
    } else {
        front_face = a < 0 ? true : false;
    }
    if (enable_cullface_) {
        if ((cull_face_opt_ == GL_BACK) && front_face ||
            (cull_face_opt_ == GL_FRONT) && !front_face) {
            OutputData(front_face);
        }
    } else {
        OutputData(front_face);
    }
}

void Cullface::OutputData(bool front_face) {
    for (int i = 0; i < 3; ++i) {
        point_attri_vec_[i].front_face = front_face;
        viewport_.PushData(point_attri_vec_[i]);
    }
}