#include "raster.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "utils.h"
#include "global_config.h"
#include "pipeline_utils.h"

Raster::Raster() {}

Raster::~Raster() {}

void Raster::SetPrimitiveType(int type) {
    primitive_type_ = type;
}

void Raster::SetAttributeNumber(uint32_t number) {
    arrtibute_number_ = number;
}

void Raster::SetRasterConfig(const RasterConfig& config) {
    line_config_.linewidth = config.linewidth;
}

void Raster::SetViewportConfig(const ViewportConfig& config) {
    viewport_x_ = config.x;
    viewport_y_ = config.y;
    viewport_w_ = config.width - 1;
    viewport_h_ = config.height - 1;
}

void Raster::PushData(const Point& point_data) {
    attribute_data_vec_.push_back(point_data);
    uint32_t point_count = GetPrimitivePointCount(primitive_type_);
    if (attribute_data_vec_.size() == point_count) {
        switch (primitive_type_) {
        case GL_POINTS: 
            RasterPoint();
            break;
        case GL_LINES:
        case GL_LINE_STRIP:
        case GL_LINE_LOOP: 
            RasterLine();
            break;
        case GL_TRIANGLES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN: 
            // RasterTriangleLinear();
            RasterTriangleBarycentric();
            break;
        default: break;
        }
        attribute_data_vec_.clear();
    }
}

void Raster::Swap(Point& a, Point& b) {
    Point tmp = a;
    a = b;
    b = tmp;
}

void Raster::DoRaster(const Point& left, const Point& right) {
    int length = right.point_position.x - left.point_position.x + 1;
    for (int i = 0; i < length; ++i) {
        float weight = i * 1.0 / length;
        Point result = Lerp(left, right, weight);
        if (primitive_type_ == GL_POINTS) {
            result.point_coord.x = point_config_.coord_step * i;
        }
        if (CheckPoint(result)) {
            pixel_processor_.PushData(result);
        }
    }
}

void Raster::RasterPoint() {
    Point point = attribute_data_vec_[0];
    Point left, right;
    point_config_.coord_step = 1.0;
    point.point_size = point.point_size > 255 ? 255 : point.point_size;
    int left_incre = (point.point_size - 1) / 2 + (point.point_size - 1) % 2;
    int right_incre = (point.point_size - 1) / 2;

    // std::cout << "point size: " << point.point_size << std::endl;
    // std::cout << "left_incre: " << left_incre << std::endl;
    // std::cout << "right_incre: " << right_incre << std::endl;

    if (point.point_position.x + right_incre < viewport_x_ || 
        point.point_position.x - left_incre > viewport_w_ ||
        point.point_position.y + right_incre < viewport_y_ ||
        point.point_position.y - left_incre > viewport_h_) {
        return;
    }

    if (point.point_size != 1) {
        point_config_.coord_step = 1.0 / (point.point_size - 1);
    }
    for (int i = -left_incre; i <= right_incre; ++i) {
        left = point;
        left.point_position.x = left.point_position.x - left_incre;
        left.point_position.y = left.point_position.y + i;
        left.point_coord.y = point_config_.coord_step * (i + left_incre);
        right = left;
        right.point_position.x = point.point_position.x + right_incre;
        DoRaster(left, right);
    }
}

void Raster::RasterLine() {
    Point points[2];
    points[0] = attribute_data_vec_[0];
    points[1] = attribute_data_vec_[1];
    Point left, right;
    if (points[0].point_position.x > points[1].point_position.x) {
        Swap(points[0], points[1]);
    }
/*
    std::cout << points[0].point_position.x << " " << points[0].point_position.y << " "
              << points[0].point_position.z << " " << points[0].point_position.w << " "
              << points[0].point_attributes[0].attri[0] << " "
              << points[0].point_attributes[0].attri[1] << " "
              << points[0].point_attributes[0].attri[2] << " "
              << points[0].point_attributes[0].attri[3] << std::endl;
    std::cout << points[1].point_position.x << " " << points[1].point_position.y << " "
              << points[1].point_position.z << " " << points[1].point_position.w << " "
              << points[1].point_attributes[0].attri[0] << " "
              << points[1].point_attributes[0].attri[1] << " "
              << points[1].point_attributes[0].attri[2] << " "
              << points[1].point_attributes[0].attri[3] << std::endl;
*/
    if (points[0].point_position.x == points[1].point_position.x) {
        if (points[0].point_position.y > points[1].point_position.y) {
            Swap(points[0], points[1]);
        }
        int length = points[1].point_position.y - points[0].point_position.y;
        for (int i = 0; i < length; ++i) {
            float weight = i * 1.0 / length;
            left = Lerp(points[0], points[1], weight);
            left.point_position.y = points[0].point_position.y + i;
            right = left;
            right.point_position.x += line_config_.linewidth;
            DoRaster(left, right);
        }
        return;
    }
    if (points[0].point_position.y == points[1].point_position.y) {
        for (int i = 0; i < line_config_.linewidth; ++i) {
            left = points[0];
            left.point_position.y += i;
            right = points[1];
            right.point_position.y += i;
            DoRaster(left, right);
        }
        return;
    }
    line_config_.line_k = (points[1].point_position.y - points[0].point_position.y) * 1.0 /
                          (points[1].point_position.x - points[0].point_position.x);
    line_config_.line_b = points[0].point_position.y - line_config_.line_k * points[0].point_position.x;

    float y_dis = fabs(points[1].point_position.y - points[0].point_position.y);
    float x_dis = fabs(points[1].point_position.x - points[0].point_position.x);
    if (y_dis >= x_dis) {
        line_config_.y_increase = true;
    } else {
        line_config_.y_increase = false;
    }
    if (line_config_.y_increase) {
        // std::cout << "y_increase" << std::endl;
        for (int i = 0; i < y_dis; ++i) {
            float weight = i * 1.0 / y_dis;
            left = Lerp(points[0], points[1], weight);
            if (line_config_.line_k > 0) {
                left.point_position.y = points[0].point_position.y + i;
            } else {
                left.point_position.y = points[0].point_position.y - i;
            }
            left.point_position.x = (int)(left.point_position.y - line_config_.line_b) * 1.0 / line_config_.line_k;
            right = left;
            right.point_position.x = right.point_position.x + line_config_.linewidth;
            DoRaster(left, right);
        }
    } else {
        // std::cout << "x_increase" << std::endl;
        for (int i = 0; i < x_dis; ++i) {
            float weight = i * 1.0 / x_dis;
            left = Lerp(points[0], points[1], weight);
            left.point_position.x = points[0].point_position.x + i;
            left.point_position.y = (int)(line_config_.line_k * left.point_position.x + line_config_.line_b);
            right = left;
            right.point_position.x = right.point_position.x + line_config_.linewidth;
            DoRaster(left, right);
        }
    }
}

void Raster::RasterTriangleLinear() {
    Point points[3];
    for (int i = 0; i < 3; ++i) {
        points[i] = attribute_data_vec_[i];
    }
    triangle_config_.need_handle_down = false;
    triangle_config_.need_handle_up = false;

    if (points[0].point_position.y > points[1].point_position.y) {
        Swap(points[0], points[1]);
    }
    if (points[1].point_position.y > points[2].point_position.y) {
        Swap(points[1], points[2]);
    }
    if (points[0].point_position.y > points[1].point_position.y) {
        Swap(points[0], points[1]);
    }

    if (points[1].point_position.y == points[2].point_position.y) {
        triangle_config_.need_handle_down = true;
        triangle_config_.down_triangle.points[0] = points[1];
        triangle_config_.down_triangle.points[1] = points[2];
        triangle_config_.down_triangle.points[2] = points[0];
    } else if (points[1].point_position.y == points[0].point_position.y) {
        triangle_config_.need_handle_up = true;
        triangle_config_.up_triangle.points[0] = points[1];
        triangle_config_.up_triangle.points[1] = points[0];
        triangle_config_.up_triangle.points[2] = points[2];
    } else {
        triangle_config_.need_handle_down = true;
        triangle_config_.need_handle_up = true;
        double weight = (points[2].point_position.y - points[1].point_position.y) /
                        (points[2].point_position.y - points[0].point_position.y);

        Point edge_point = Lerp(points[2], points[0], weight);
        triangle_config_.up_triangle.points[0] = points[1];
        triangle_config_.up_triangle.points[1] = edge_point;
        triangle_config_.up_triangle.points[2] = points[2];
        triangle_config_.down_triangle.points[0] = points[1];
        triangle_config_.down_triangle.points[1] = edge_point;
        triangle_config_.down_triangle.points[2] = points[0];
    }
    if (triangle_config_.need_handle_up) {
        RasterUpTriangleLinear();
    } else if (triangle_config_.need_handle_down) {
        RasterDownTriangleLinear();
    }
}

void Raster::RasterUpTriangleLinear() {
    Point up_left, up_right, up_top;
    up_left = triangle_config_.up_triangle.points[0].point_position.x > triangle_config_.up_triangle.points[1].point_position.x ?
              triangle_config_.up_triangle.points[1] : triangle_config_.up_triangle.points[0];
    up_right = triangle_config_.up_triangle.points[0].point_position.x > triangle_config_.up_triangle.points[1].point_position.x ?
               triangle_config_.up_triangle.points[0] : triangle_config_.up_triangle.points[1];
    up_top = triangle_config_.up_triangle.points[2];
    int len = up_top.point_position.y - up_left.point_position.y;
    Point left, right;
    for (int i = 0; i < len; ++i) {
        float weight = (float)i / len;
        left = Lerp(up_top, up_left, weight);
        right = Lerp(up_top, up_right, weight);
        left.point_position.y = right.point_position.y = up_top.point_position.y - i;
        DoRaster(left, right);
    }
}

void Raster::RasterDownTriangleLinear() {
    Point down_left, down_right, down_bottom;
    down_left = triangle_config_.down_triangle.points[0].point_position.x > triangle_config_.down_triangle.points[1].point_position.x ?
                triangle_config_.down_triangle.points[1] : triangle_config_.down_triangle.points[0];
    down_right = triangle_config_.down_triangle.points[0].point_position.x > triangle_config_.down_triangle.points[1].point_position.x ?
                 triangle_config_.down_triangle.points[0] : triangle_config_.down_triangle.points[1];
    down_bottom = triangle_config_.down_triangle.points[2];
    int len = down_left.point_position.y - down_bottom.point_position.y;
    Point left, right;
    for (int i = 0; i < len; ++i) {
        float weight = (float)i / len;
        left = Lerp(down_left, down_bottom, weight);
        right = Lerp(down_right, down_bottom, weight);
        left.point_position.y = right.point_position.y = down_left.point_position.y - i;
        DoRaster(left, right);
    }
}

void Raster::RasterTriangleBarycentric() {
    // auto start_time = GetTimeStamp();
    Point points[3];
    for (int i = 0; i < 3; ++i) {
        points[i] = attribute_data_vec_[i];
    }
    int min_x = std::min(std::min(points[0].point_position.x, points[1].point_position.x), points[2].point_position.x);
    int min_y = std::min(std::min(points[0].point_position.y, points[1].point_position.y), points[2].point_position.y);
    int max_x = std::max(std::max(points[0].point_position.x, points[1].point_position.x), points[2].point_position.x);
    int max_y = std::max(std::max(points[0].point_position.y, points[1].point_position.y), points[2].point_position.y);
    float area = (points[0].point_position.x * points[1].point_position.y - points[0].point_position.y * points[1].point_position.x) +
                 (points[1].point_position.x * points[2].point_position.y - points[1].point_position.y * points[2].point_position.x) +
                 (points[2].point_position.x * points[0].point_position.y - points[2].point_position.y * points[0].point_position.x);

    int coef = area > 0 ? 1 : -1;
    float cross[3];
    float cross_diff_x[3];
    float cross_diff_y[3];
    float barycentric_coord[3];
    float barycentric_diff_x[3];
    float barycentric_diff_y[3];
    PointPosition p(min_x, min_y, 0, 1.0);
    for (int i = 0; i < 3; ++i) {
        cross[i] = VecCross(points[i].point_position, points[(i + 1) % 3].point_position, p);
        cross_diff_x[i] = points[(i + 1) % 3].point_position.x - points[i].point_position.x;
        cross_diff_y[i] = points[i].point_position.y - points[(i + 1) % 3].point_position.y;
        barycentric_coord[i] = cross[i] / area;
        barycentric_diff_x[i] = cross_diff_x[i] / area;
        barycentric_diff_y[i] = cross_diff_y[i] / area;
    }
    int diff_y = 0;
    int diff_x = 0;
    if (min_y < viewport_y_) {
        diff_y = viewport_y_ - min_y;
        for (int i = 0; i < 3; ++i) {
            cross[i] += cross_diff_x[i] * diff_y;
            barycentric_coord[i] += barycentric_diff_x[i]  * diff_y;
        }
        min_y = viewport_y_;
    }
    if (min_x < viewport_x_) {
        diff_x = viewport_x_ - min_x;
        min_x = viewport_x_;
    }
    for (int y = min_y; y <= max_y; ++y) {
        if (y >= (viewport_y_ + viewport_h_)) {
            break;
        }
        float x_cross[3];
        float x_barycentric[3];
        for (int i = 0; i < 3; ++i) {
            x_cross[i] = cross[i] + cross_diff_y[i] * diff_x;
            x_barycentric[i] = barycentric_coord[i] + barycentric_diff_y[i] * diff_x;
        }
        int start = INT32_MAX;
        for (int i = 0; i < 3; ++i) {
            if (cross_diff_y[i] != 0) {
                int tmp = x_cross[i] / (-1 * cross_diff_y[i]);
                if (tmp < start) {
                    start = tmp;
                }
            }
        }
        // if (start < 0) {
        //     start = 0;
        // }
        for (int i = 0; i < 3; ++i) {
            x_cross[i] += cross_diff_y[i] * (start - min_x);
            x_barycentric[i] += barycentric_diff_y[i] * (start - min_x);
        }
        bool tmp_inside = false;
        for (int x = start; x <= max_x; ++x) {
            if (x >= (viewport_x_ + viewport_w_)) {
                break;
            }
            bool inside = true;
            for (int i = 0; i < 3; ++i) {
                if (x_cross[i] * coef < 0) {
                    inside = false;
                    break;
                }
            }
            if (inside) {
                if (!tmp_inside) {
                    tmp_inside = true;
                }
                PointPosition p(x, y);
                Point point = Lerp(p, points, x_barycentric);
                pixel_processor_.PushData(point);
            } else {
                if (tmp_inside) {
                    break;
                }
            }
            for (int i = 0; i < 3; ++i) {
                x_cross[i] += cross_diff_y[i];
                x_barycentric[i] += barycentric_diff_y[i];
            }
        }
        for (int i = 0; i < 3; ++i) {
            cross[i] += cross_diff_x[i];
            barycentric_coord[i] += barycentric_diff_x[i];
        }
    }
    // auto end_time = GetTimeStamp();
    // std::cout << "raster time: " << end_time - start_time << std::endl;
}

void Raster::RasterTriangleBarycentricBlock() {
    Point points[3];
    for (int i = 0; i < 3; ++i) {
        points[i] = attribute_data_vec_[i];
    }
    int min_x = std::min(std::min(points[0].point_position.x, points[1].point_position.x), points[2].point_position.x);
    int min_y = std::min(std::min(points[0].point_position.y, points[1].point_position.y), points[2].point_position.y);
    int max_x = std::max(std::max(points[0].point_position.x, points[1].point_position.x), points[2].point_position.x);
    int max_y = std::max(std::max(points[0].point_position.y, points[1].point_position.y), points[2].point_position.y);
    int incre_x = 4;
    int incre_y = 4;
    min_x = min_x & ~(incre_x - 1);
    min_y = min_y & ~(incre_y - 1);
    float area = (points[0].point_position.x * points[1].point_position.y - points[0].point_position.y * points[1].point_position.x) +
                 (points[1].point_position.x * points[2].point_position.y - points[1].point_position.y * points[2].point_position.x) +
                 (points[2].point_position.x * points[0].point_position.y - points[2].point_position.y * points[0].point_position.x);

    int coef = area > 0 ? 1 : -1;
    float cross[3];
    float cross_diff_x[3];
    float cross_diff_y[3];
    float barycentric_coord[3];
    float barycentric_diff_x[3];
    float barycentric_diff_y[3];
    PointPosition p(min_x, min_y, 0, 1.0);
    for (int i = 0; i < 3; ++i) {
        cross[i] = VecCross(points[i].point_position, points[(i + 1) % 3].point_position, p);
        cross_diff_x[i] = points[(i + 1) % 3].point_position.x - points[i].point_position.x;
        cross_diff_y[i] = points[i].point_position.y - points[(i + 1) % 3].point_position.y;
        barycentric_coord[i] = cross[i] / area;
        barycentric_diff_x[i] = cross_diff_x[i] / area;
        barycentric_diff_y[i] = cross_diff_y[i] / area;
    }

    for (int y = min_y; y <= max_y; y += incre_y) {
        for (int x = min_x; x <= max_x; x += incre_x) {
            int offset = x - min_x;
            float block_cross[3];
            float block_barycentric_coord[3];
            for (int i = 0; i < 3; ++i) {
                block_cross[i] = cross[i] + cross_diff_y[i] * offset;
                block_barycentric_coord[i] = barycentric_coord[i] + barycentric_diff_y[i] * offset;
            }
            float cross_bound[4][3];
            for (int i = 0; i < 3; ++i) {
                cross_bound[0][i] = block_cross[i];
                cross_bound[1][i] = cross_bound[0][i] + cross_diff_y[i] * 3;
                cross_bound[2][i] = cross_bound[0][i] + cross_diff_x[i] * 3;
                cross_bound[3][i] = cross_bound[2][i] + cross_diff_y[i] * 3;
            }
            int inside_count = 0;
            for (int i = 0; i < 4; ++i) {
                bool inside = true;
                for (int j = 0; j < 3; ++j) {
                    if (cross_bound[i][j] * coef < 0) {
                        inside = false;
                    }
                }
                if (inside) {
                    inside_count++;
                }
            }
            if (inside_count == 4) {
                for (int j = 0; j < 4; ++j) {
                    for (int i = 0; i < 4; ++i) {
                        PointPosition p(x + i, y + j);
                        float barycentric[3];
                        for (int k = 0; k < 3; ++k) {
                            barycentric[k] = block_barycentric_coord[k] + i * barycentric_diff_y[k] + j * barycentric_diff_x[k];
                        }
                        Point point = Lerp(p, points, barycentric);
                        if (CheckPoint(point)) {
                            pixel_processor_.PushData(point);
                        }
                    }
                }
            }
            else if (inside_count > 0) {
                for (int j = 0; j < 4; ++j) {
                    for (int i = 0; i < 4; ++i) {
                        int tmp_cross[3];
                        float barycentric[3];
                        bool inside = true;
                        for (int k = 0; k < 3; ++k) {
                            barycentric[k] = block_barycentric_coord[k] + i * barycentric_diff_y[k] + j * barycentric_diff_x[k];
                            tmp_cross[k] = block_cross[k] + i * cross_diff_y[k] + j * cross_diff_x[k];
                            tmp_cross[k] = tmp_cross[k] * coef;
                            inside = (tmp_cross[k] >= 0) && inside;
                        }
                        if (inside) {
                            PointPosition p(x + i, y + j);
                            Point point = Lerp(p, points, barycentric);
                            if (CheckPoint(point)) {
                                pixel_processor_.PushData(point);
                            }
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 3; ++i) {
            cross[i] += cross_diff_x[i] * incre_y;
            barycentric_coord[i] += barycentric_diff_x[i] * incre_y;
        }
    }
}

bool Raster::CheckPoint(const Point& point) {
    bool result = true;
    if (point.point_position.x < viewport_x_ || point.point_position.x > (viewport_x_ + viewport_w_)) {
        result = false;
    }
    if (point.point_position.y < viewport_y_ || point.point_position.y > (viewport_y_ + viewport_h_)) {
        result = false;
    }
    return result;
}

bool Raster::CheckPoint(const PointPosition& pos) {
    bool result = true;
    if (pos.x < viewport_x_ || pos.x > (viewport_x_ + viewport_w_)) {
        result = false;
    }
    if (pos.y < viewport_y_ || pos.y > (viewport_y_ + viewport_h_)) {
        result = false;
    }
    return result;
}