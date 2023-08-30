#pragma once
#include <vector>

#include "gl_common.h"
#include "gl_utils.h"

enum PrimitiveType {
    kPrimitiveTypeUnknow,
    kPrimitiveTypePoints,
    kPrimitiveTypeLines,
    kPrimitiveTypeLineStrip,
    kPrimitiveTypeLineLoop,
    kPrimitiveTypeTriangles,
    kPrimitiveTypeTriangleStrip,
    kPrimitiveTypeTriangleFan,
};

struct PointData {
    // int number;
    int first{};
    int count{};
    int mode{};
    std::vector<int> stride{};
    std::vector<std::vector<float>> data{};
};

struct AttributeConfig {
    int size;
    int stride;
    void* pointer;
    bool enable;
};

struct PointPosition {
    float x{};
    float y{};
    float z{};
    float w{};

    PointPosition() {
        x = 0;
        y = 0;
        z = 0;
        w = 1.0;
    }

    PointPosition(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    PointPosition(float x, float y) {
        this->x = x;
        this->y = y;
        this->z = 0;
        this->w = 1.0;
    }
    
    inline PointPosition operator*(float val) const {
        PointPosition out;
        out.x = this->x * val;
        out.y = this->y * val;
        out.z = this->z * val;
        out.w = this->w * val;
        return out;
    }

    inline PointPosition operator+(const PointPosition& val) const {
        PointPosition out;
        out.x = val.x + x;
        out.y = val.y + y;
        out.z = val.z + z;
        out.w = val.w + w;
        return out;
    }
};

struct PointCoord {
    float x{};
    float y{};
};

struct PointAttribute {
    float attri[4]{};
    inline PointAttribute operator*(float val) const {
        PointAttribute out;
        out.attri[0] = attri[0] * val;
        out.attri[1] = attri[1] * val;
        out.attri[2] = attri[2] * val;
        out.attri[3] = attri[3] * val;
        return out;
    }

    inline PointAttribute operator+(const PointAttribute& val) const {
        PointAttribute out;
        out.attri[0] = attri[0] + val.attri[0];
        out.attri[1] = attri[1] + val.attri[1];
        out.attri[2] = attri[2] + val.attri[2];
        out.attri[3] = attri[3] + val.attri[3];
        return out;
    }

    inline bool operator==(const PointAttribute& rhs) const {
        bool result = true;
        for (int i = 0; i < 4; ++i) {
            if (rhs.attri[i] != attri[i]) {
                result = false;
                break;
            }
        }
        return result;
    }
};


struct Point {
    PointPosition point_position;
    PointCoord point_coord;
    std::vector<PointAttribute> point_attributes{};
    bool front_face{true};
    int point_size{1};
};

struct Primitive {
    Point points[3];
};

struct PixelAttri {
    uint16_t x{};
    uint16_t y{};
    uint32_t z{};
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
    uint8_t a{};
    bool front_face{};
};

struct Color {
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
    uint8_t a{};

    Color operator+(const Color& tmp) {
        Color color;
        color.r = Clamp(r + tmp.r, 0, 255);
        color.g = Clamp(g + tmp.g, 0, 255);
        color.b = Clamp(b + tmp.b, 0, 255);
        color.a = Clamp(a + tmp.a, 0, 255);
        return color;
    }

    Color operator-(const Color& tmp) {
        Color color;
        color.r = Clamp(r - tmp.r, 0, 255);
        color.g = Clamp(g - tmp.g, 0, 255);
        color.b = Clamp(b - tmp.b, 0, 255);
        color.a = Clamp(a - tmp.a, 0, 255);
        return color;
    }

    Color operator*(int coe) {
        Color color;
        color.r = Clamp(r * coe, 0, 255);
        color.g = Clamp(g * coe, 0, 255);
        color.b = Clamp(b * coe, 0, 255);
        color.a = Clamp(a * coe, 0, 255);
        return color;
    }

    Color operator/(int coe) {
        Color color;
        color.r = Clamp(r / coe, 0, 255);
        color.g = Clamp(g / coe, 0, 255);
        color.b = Clamp(b / coe, 0, 255);
        color.a = Clamp(a / coe, 0, 255);
        return color;
    }
};