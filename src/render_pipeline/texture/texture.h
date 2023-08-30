#pragma once

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#include "common.h"
#include "pipeline_struct.h"
#include "texture_uncompress.h"

class Texture {
public:
    Texture();
    ~Texture();

    void SetMode(int key, int value);
    void TexImage2D(GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * data);
    void TexSubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void * data);
    void CompressedTexImage2D(GLint level, GLenum format, GLsizei width, GLsizei height, GLsizei imageSize, const void * data);
    void CompressedTexSubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLsizei imageSize, const void * data);
    void CopyTexImage2D(GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
    void CopyTexSubImage2D(GLint level, GLint xoffset,  GLint yoffset, GLint x, GLint y, GLsizei width,  GLsizei height);
    void PixelStorei(GLenum pname, GLint param);
    void GenerateMipmap();
    Color GetData(float u, float v, int level);
    uint32_t GetData(int addr, int level);
    void SetData(int addr, int data, int level);

private:
    float CalcPosition(float pos, int mode);
    Color GetColor(int index, int format, int level);
    // Color GetCompressData(float u, float v);

private:
    int width_{};
    int height_{};
    int format_{};
    int level_{};
    int type_{};
    int pixel_storei_{};

    TextureDecoder texture_uncompress_{};
    std::vector<uint8_t> compress_data_{};
    std::unordered_map<int, int> mode_map_{};
    std::unordered_map<int, std::vector<uint8_t>> data_map_{};
};