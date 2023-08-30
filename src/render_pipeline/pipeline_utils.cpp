#include "pipeline_utils.h"

#include <iostream>

uint32_t Min(uint32_t num_a, uint32_t num_b) {
    return num_a < num_b ? num_a : num_b; 
}

uint32_t GetPrimitivePointCount(int type) {
    switch (type) {
    case GL_POINTS:
        return 1; 
        break;
    case GL_LINES:
    case GL_LINE_STRIP:
    case GL_LINE_LOOP:
        return 2;
        break;
    case GL_TRIANGLES:
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
        return 3;
        break;
    default:
        break;
    }
    return 0;
}

float VecCross(PointPosition p1, PointPosition p2, PointPosition p3) {
    float cross = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
    return cross;
}

Point Lerp(const PointPosition& point_position, Point points[3], float barycentric[3]) {
    Point result;
    result.front_face = points[0].front_face;
    result.point_position = point_position;
    std::vector<float> bary{barycentric[1], barycentric[2], barycentric[0]};
    for (int i = 0; i < 3; ++i) {
        bary[i] = bary[i] * points[i].point_position.w * points[i].point_position.z;
    }
    result.point_position.z = 1.0f / (bary[0] + bary[1] + bary[2]);
    for (int i = 0; i < 3; ++i) {
        bary[i] *= result.point_position.z;
    }
    int attri_count = points[0].point_attributes.size();
    result.point_attributes.resize(attri_count);
    for (int i = 0; i < attri_count; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.point_attributes[i].attri[j] = 
                bary[0] * points[0].point_attributes[i].attri[j] +
                bary[1] * points[1].point_attributes[i].attri[j] +
                bary[2] * points[2].point_attributes[i].attri[j];
        }
    }
    result.point_position.z = barycentric[1] * points[0].point_position.z +
                              barycentric[2] * points[1].point_position.z +
                              barycentric[0] * points[2].point_position.z;
    // result.point_position.z = bary[0] * points[0].point_position.z +
    //                           bary[1] * points[1].point_position.z +
    //                           bary[2] * points[2].point_position.z;
    return result;
}

Point Lerp(const Point& left, const Point& right, float factor) {
    Point result;
    result.front_face = left.front_face;
    result.point_position.x = (int)(left.point_position.x * (1.0f - factor) + right.point_position.x * factor + 0.001);
    result.point_position.y = (int)(left.point_position.y * (1.0f - factor) + right.point_position.y * factor + 0.001);
    result.point_position.z = left.point_position.z * (1.0f - factor) + right.point_position.z * factor;
    result.point_position.w = left.point_position.w * (1.0f - factor) + right.point_position.w * factor;
    int attri_count = left.point_attributes.size();
    result.point_attributes.resize(attri_count);
    for (int i = 0; i < attri_count; ++i) {
        if (left.point_attributes[i] == right.point_attributes[i]) {
            result.point_attributes[i] = left.point_attributes[i];
        } else {
            result.point_attributes[i] = left.point_attributes[i] * (1.0f - factor) + right.point_attributes[i] * factor;
        }
        // result.point_attributes[i] = left.point_attributes[i] * (1.0f - factor) + right.point_attributes[i] * factor;
    }
    return result;
}