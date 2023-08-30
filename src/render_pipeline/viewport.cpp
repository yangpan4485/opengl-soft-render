#include "viewport.h"

#include <iostream>

#include "global_config.h"

ViewPort::ViewPort() {}

ViewPort::~ViewPort() {}

void ViewPort::SetPrimitiveType(int type) {
    primitive_type_ = type;
    raster_.SetPrimitiveType(type);
}

void ViewPort::SetAttributeNumber(uint32_t number) {
    arrtibute_number_ = number;
    raster_.SetAttributeNumber(number);
}

void ViewPort::SetViewportConfig(const ViewportConfig& config) {
    window_x_ = config.x;
    window_y_ = config.y;
    window_width_ = config.width;
    window_height_ = config.height;
    near_ = config.near;
    far_ = config.far;
    raster_.SetViewportConfig(config);
}

void ViewPort::SetRasterConfig(const RasterConfig& config) {
    raster_.SetRasterConfig(config);
}

void ViewPort::PushData(const Point& point_data) {
    if (primitive_type_ == GL_POINTS && point_data.point_position.w == 0) {
        return;
    }
    float tmp_x = window_x_ + window_width_ * 1.0 / 2;
    float tmp_y = window_y_ + window_height_ * 1.0 / 2;
    Point output = point_data;
    float z = output.point_position.z;
    output.point_position.x = output.point_position.x / output.point_position.w;
    output.point_position.y = output.point_position.y / output.point_position.w;
    output.point_position.z = output.point_position.z / output.point_position.w;

    output.point_position.x = (int)((window_width_ / 2) * output.point_position.x + tmp_x);
    output.point_position.y = (int)((window_height_ / 2) * output.point_position.y + tmp_y);
    output.point_position.z = (far_ - near_) / 2 * output.point_position.z + (far_ + near_) / 2;
    output.point_position.w = 1.0 / output.point_position.w;
    // std::cout << output.point_position.x << ", " << output.point_position.y << ", " << output.point_position.z << ", "
    //           << output.point_position.w << std::endl;
    // std::cout << output.point_attributes[0].attri[0] << ", " << output.point_attributes[0].attri[1] << std::endl;
    raster_.PushData(output);
}