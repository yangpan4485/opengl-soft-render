#pragma once

#include <cstdint>
#include "gl_common.h"
#include "pipeline_struct.h"

uint32_t Min(uint32_t num_a, uint32_t num_b);
uint32_t GetPrimitivePointCount(int type);

float VecCross(PointPosition p1, PointPosition p2, PointPosition p3);
Point Lerp(const PointPosition& point_position, Point points[3], float barycentric[3]);
Point Lerp(const Point& left, const Point& right, float factor);