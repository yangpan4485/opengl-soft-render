#include "pa.h"
#include <iostream>

PrimitiveAssembly::PrimitiveAssembly() {}

PrimitiveAssembly::~PrimitiveAssembly() {}

void PrimitiveAssembly::SetPrimitiveType(int type) {
    primitive_type_ = type;
    clip.SetPrimitiveType(type);
}

void PrimitiveAssembly::SetAttributeNumber(uint32_t number) {
    arrtibute_number_ = number;
    clip.SetAttributeNumber(number);
}

void PrimitiveAssembly::SetCullfaceConfig(const CullfaceConfig& config) {
    clip.SetCullfaceConfig(config);
}

void PrimitiveAssembly::SetViewportConfig(const ViewportConfig& config) {
    clip.SetViewportConfig(config);
}

void PrimitiveAssembly::SetRasterConfig(const RasterConfig& config) {
    clip.SetRasterConfig(config);
}

void PrimitiveAssembly::PushData(const Point& point_data, bool is_end) {
    // std::cout << "pa: " << point_data.point_position.x << " " << point_data.point_position.y 
    //           << " " << point_data.point_position.z << " " << point_data.point_position.w << std::endl;
    // std::cout << "primitive_type_: " << primitive_type_ << std::endl;
    if (primitive_type_ == GL_LINE_STRIP || primitive_type_ == GL_LINE_LOOP) {
        attribute_data_queue_.push(point_data);
        if (attribute_data_queue_.size() == 2) {
            HandleLineData(is_end);
        }
    } else if (primitive_type_ == GL_TRIANGLE_STRIP || primitive_type_ == GL_TRIANGLE_FAN) {
        attribute_data_queue_.push(point_data);
        if (attribute_data_queue_.size() == 3) {
            HandleTriangleData(is_end);
        }
    } else {
        clip.PushData(point_data);
    }
    // std::cout << "is_end: " << is_end << std::endl;
    if (is_end) {
        while(!attribute_data_queue_.empty()) {
            attribute_data_queue_.pop();
        }
    }
}

void PrimitiveAssembly::HandleLineData(bool is_end) {
    std::vector<Point> points{2};
    for (int i = 0; i < 2; ++i) {
        points[i] = attribute_data_queue_.front();
        attribute_data_queue_.pop();
        clip.PushData(points[i]);
    }
    if (line_loop_first_ && primitive_type_ == GL_LINE_LOOP) {
        line_loop_first_point_ = points[0];
        line_loop_first_ = false;
    }
    
    if (is_end) {
        if (primitive_type_ == GL_LINE_LOOP) {
            line_loop_first_ = true;
            clip.PushData(points[1]);
            clip.PushData(line_loop_first_point_);
        }
    } else {
        attribute_data_queue_.push(points[1]);
    }
}

void PrimitiveAssembly::HandleTriangleData(bool is_end) {
    std::vector<Point> points{3};
    for (int i = 0; i < 3; ++i) {
        points[i] = attribute_data_queue_.front();
        attribute_data_queue_.pop();
        clip.PushData(points[i]);
    }
    if (is_end) {
        triangle_strip_flag_ = false;
        return;
    }
    if (primitive_type_ == GL_TRIANGLE_STRIP) {
        if (triangle_strip_flag_) {
            attribute_data_queue_.push(points[0]);
            attribute_data_queue_.push(points[2]);
        } else {
            attribute_data_queue_.push(points[2]);
            attribute_data_queue_.push(points[1]);
        }
        triangle_strip_flag_ = !triangle_strip_flag_;
    } else if (primitive_type_ == GL_TRIANGLE_FAN) {
        attribute_data_queue_.push(points[0]);
        attribute_data_queue_.push(points[2]);
    }
}